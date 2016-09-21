#include "stdafx.h"
#include "NoiseProgram.h"
#include <resource.h>

using DirectX::SimpleMath::Matrix;

HRESULT CNoiseProgram::initialize()
{
	CHECK( m_vs.load( IDR_VERT, GL_VERTEX_SHADER ) );
	CHECK( m_ps.load( IDR_FRAG, GL_FRAGMENT_SHADER ) );

	// Create a shader program object.
	m_prog = glCreateProgram();

	// Attach the vertex and fragment shader to the program object.
	glAttachShader( m_prog, m_vs.nativeObject );
	glAttachShader( m_prog, m_ps.nativeObject );

	// Bind shader input variables.
	// The names must exactly match "in" variables in GLSL.
	// The indices must exactly match what's specified in glVertexAttribPointer inside initializeFormat for the vertex structure.
	glBindAttribLocation( m_prog, 0, "inputPosition" );
	glBindAttribLocation( m_prog, 1, "inputTexcoord" );

	// Link VS + PS together into a program
	glLinkProgram( m_prog );
	int status;
	glGetProgramiv( m_prog, GL_LINK_STATUS, &status );
	if( status != 1 )
	{
		GLint maxLength = 0;
		glGetProgramiv( m_prog, GL_INFO_LOG_LENGTH, &maxLength );

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog( maxLength );
		glGetProgramInfoLog( m_prog, maxLength, &maxLength, &infoLog[ 0 ] );

		CStringA message( infoLog.data(), maxLength );
		OutputDebugStringA( message );

		//We don't need the shader anymore.
		glDeleteProgram( m_prog );

		return E_FAIL;
	}

	// Resolve constant names: the profiler showed glGetUniformLocation is very slow, at least here on Windows 10 and GTX 960
	// The names must exactly match "uniform" variables in GLSL.
	world = glGetUniformLocation( m_prog, "worldMatrix" );
	view = glGetUniformLocation( m_prog, "viewMatrix" );
	proj = glGetUniformLocation( m_prog, "projectionMatrix" );
	shaderTexture = glGetUniformLocation( m_prog, "shaderTexture" );
	noiseParameters = glGetUniformLocation( m_prog, "noiseParameters" );

	if( -1 == world || -1 == view || -1 == proj || -1 == shaderTexture || -1 == noiseParameters )
		return E_FAIL;

	return S_OK;
}

void CNoiseProgram::destroy()
{
	if( 0 != m_prog )
	{
		// Detach the vertex and fragment shaders from the program.
		glDetachShader( m_prog, m_vs.nativeObject );
		glDetachShader( m_prog, m_ps.nativeObject );

		// Delete the shader program.
		glDeleteProgram( m_prog );
		m_prog = 0;
	}
	m_ps.destroy();
	m_vs.destroy();
}

void CNoiseProgram::setTexture( int nTextureUnit )
{
	glUniform1i( shaderTexture, nTextureUnit );
}

// Return pointer to the first element of the matrix
static inline const float* ptr( const Matrix& m )
{
	return &m.m[ 0 ][ 0 ];
}

void CNoiseProgram::setTransforms( const Matrix& worldMatrix, const Matrix& viewMatrix, const Matrix& projectionMatrix ) const
{
	glUniformMatrix4fv( world, 1, false, ptr( worldMatrix ) );
	glUniformMatrix4fv( view, 1, false, ptr( viewMatrix ) );
	glUniformMatrix4fv( proj, 1, false, ptr( projectionMatrix ) );
}

void CNoiseProgram::setNoiseParameters( Vector2 noiseXyScale, float noiseResultScale, float time ) const
{
	glUniform4f( noiseParameters, noiseXyScale.x, noiseXyScale.y, noiseResultScale, time );
}