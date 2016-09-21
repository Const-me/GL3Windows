#pragma once

struct DDS_HEADER;

// OpenGL texture, loaded from the resource.
class CTexture
{
	GLuint m_textureID;

public:
	CTexture() : m_textureID( 0 ) { }

	~CTexture()
	{
		destroy();
	}

	// Load a compressed DDS texture, from resource to GPU
	HRESULT loadCompressedDdsResource( int idTexture, UINT iResourceName, LPCTSTR resourceType );

	void bind() const
	{
		glBindTexture( GL_TEXTURE_2D, m_textureID );
	}

	void destroy();

private:
	// Get the OpenGL format constant from the DDS file header.
	// Returns ERROR_UNSUPPORTED_COMPRESSION if the DDS ain't compressed or the compression ain't DXT[1-5]
	static HRESULT getInternalCompressedFormat( const DDS_HEADER &header, GLenum &internalformat );

	// Manually decompress a DXT texture. Currently, only DXT1 is supported.
	static HRESULT decompressTexture( GLenum internalFormat, GLsizei width, GLsizei height, GLsizei imageSize, const BYTE* pData );
};