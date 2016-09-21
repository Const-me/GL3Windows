#pragma once

// OpenGL shader object
class CShader
{
	GLuint m_shader;

public:
	CShader() : m_shader( 0 ) { }
	~CShader()
	{
		destroy();
	}

	// Load and compile a shader from resource
	HRESULT load( int idResource, GLenum shaderType );

	void destroy();

	inline GLuint getNativeObject() const
	{
		return m_shader;
	}
	__declspec( property( get = getNativeObject ) ) GLuint nativeObject;
};