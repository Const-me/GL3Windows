#include "stdafx.h"
#include "Shader.h"

HRESULT CShader::load( int idResource, GLenum shaderType )
{
	std::pair<DWORD, const BYTE*> resource;
	CHECK( loadResource( MAKEINTRESOURCE( idResource ), L"glsl", resource ) );

	const GLchar *source = (const char*)resource.second;
	const GLint length = resource.first;

	GLuint shader = glCreateShader( shaderType );

	glShaderSource( shader, 1, &source, &length );
	glCompileShader( shader );

	GLint isCompiled = 0;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &isCompiled );
	if( isCompiled == GL_FALSE )
	{
		// Shader compilation failed, print fail reason to the debug output.
		GLint maxLength = 0;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog( maxLength );
		glGetShaderInfoLog( shader, maxLength, &maxLength, &infoLog[ 0 ] );

		CStringA message( infoLog.data(), maxLength );
		OutputDebugStringA( message );

		//We don't need the shader anymore.
		glDeleteShader( shader );

		return E_FAIL;
	}

	m_shader = shader;
	return S_OK;
}

void CShader::destroy()
{
	if( 0 == m_shader )
		return;
	glDeleteShader( m_shader );
	m_shader = 0;
}