#include "stdafx.h"
#include "Texture.h"
#include <d3d9types.h>
#include "decompress.h"

static const bool alwaysDecompressToRgba = false;

#pragma pack( 4 )

// Surface pixel format.
struct DDS_PIXELFORMAT
{
	DWORD dwSize;
	DWORD dwFlags;
	// Four-character codes for specifying compressed or custom formats.
	DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwABitMask;
};

// Describes a DDS file header.
struct DDS_HEADER
{
	DWORD dwSize;
	DWORD dwFlags;
	// Surface height (in pixels).
	DWORD dwHeight;
	// Surface width (in pixels).
	DWORD dwWidth;
	// The pitch or number of bytes per scan line in an uncompressed texture; the total number of bytes in the top level texture for a compressed texture.
	DWORD dwPitchOrLinearSize;
	DWORD dwDepth;
	DWORD dwMipMapCount;
	DWORD dwReserved1[ 11 ];
	DDS_PIXELFORMAT ddspf;
	DWORD dwCaps;
	DWORD dwCaps2;
	DWORD dwCaps3;
	DWORD dwCaps4;
	DWORD dwReserved2;
};

static const DWORD DDPF_ALPHAPIXELS = 0x1;

HRESULT CTexture::loadCompressedDdsResource( int idTexture, UINT iResourceName, LPCTSTR resourceType )
{
	GLint texture_units = 0;
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units );
	if( idTexture < 0 || idTexture >= texture_units )
		return E_INVALIDARG;

	std::pair<DWORD, const BYTE*> resource;
	CHECK( loadResource( MAKEINTRESOURCE( iResourceName ), resourceType, resource ) );

	glActiveTexture( GL_TEXTURE0 + idTexture );

	// Generate an ID for the texture.
	glGenTextures( 1, &m_textureID );

	// Bind the texture as a 2D texture.
	glBindTexture( GL_TEXTURE_2D, m_textureID );

	// Read and skip DDS header: OpenGL expects raw data without any headers.
	const DDS_HEADER *pHeader = (const DDS_HEADER *)( resource.second + 4 );
	const GLsizei width = pHeader->dwWidth;
	const GLsizei height = pHeader->dwHeight;

	GLenum internalFormat;
	CHECK( getInternalCompressedFormat( *pHeader, internalFormat ) );

	// OpenGL expects single mipmap here
	const GLsizei imageSize = pHeader->dwPitchOrLinearSize;

	// Skip the file header: OpenGL expects raw data without any headers.
	const BYTE* const pData = resource.second + 4 + sizeof( DDS_HEADER );

	if( alwaysDecompressToRgba || GL_FALSE == GLEW_EXT_texture_compression_s3tc )
	{
		// The hardware doesn't support S3 texture compression. Like a VmWare 12 Windows 7 guest. Need to decompress manually.
		CHECK( decompressTexture( internalFormat, width, height, imageSize, pData ) );
	}
	else
	{
		// Load the image data into the texture unit.
		glCompressedTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageSize, pData );
		CHECK( getLastGl() );
	}

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	glGenerateMipmap( GL_TEXTURE_2D );
	CHECK( getLastGl() );

	return S_OK;
}

HRESULT CTexture::getInternalCompressedFormat( const DDS_HEADER &header, GLenum &internalformat )
{
	switch( header.ddspf.dwFourCC )
	{
	case D3DFMT_DXT1:
		internalformat = ( header.ddspf.dwFlags & DDPF_ALPHAPIXELS ) ? GL_COMPRESSED_RGBA_S3TC_DXT1_EXT : GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		break;

	case D3DFMT_DXT2:
	case D3DFMT_DXT3:
		// The only difference between the two is premultiplied alpha
		internalformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;

	case D3DFMT_DXT4:
	case D3DFMT_DXT5:
		// The only difference between the two is premultiplied alpha
		internalformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;

	default:
		return HRESULT_FROM_WIN32( ERROR_UNSUPPORTED_COMPRESSION );
	}

	return S_OK;
}

void CTexture::destroy()
{
	if( 0 != m_textureID )
	{
		glDeleteTextures( 1, &m_textureID );
		m_textureID = 0;
	}
}

HRESULT CTexture::decompressTexture( GLenum internalFormat, GLsizei width, GLsizei height, GLsizei imageSize, const BYTE* pData )
{
	if( internalFormat != GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
		return HRESULT_FROM_WIN32( ERROR_UNSUPPORTED_COMPRESSION );

	if( 0 != ( ( width | height ) & 3 ) )
		return E_INVALIDARG;

	if( imageSize != width * height * 3 / 6 )
		return E_INVALIDARG;

	std::vector<BYTE> buffer;
	try
	{
		buffer.resize( width * height * 4 );
	}
	catch( std::bad_alloc& )
	{
		return E_OUTOFMEMORY;
	}

	const uint32_t stride = width * 4;
	unsigned char * const image = buffer.data();

	for( CPoint pt = CPoint( 0, 0 ); pt.y < height; pt.y += 4 )
	{
		for( pt.x = 0; pt.x < width; pt.x += 4 )
		{
			DecompressBlockBC1( pt.x, pt.y, stride, pData, image );
			pData += 8;
		}
	}

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
	CHECK( getLastGl() );

	return S_OK;
}