#pragma once
#include "Shader.h"

class CNoiseProgram
{
	// The two shaders
	CShader m_vs, m_ps;

	// OpenGL has weird concept of "shader program".
	// Unlike Direct3D, where you can mix and match VS and PS in runtime, OpenGL requires vertex and fragment shaders linked together before use, creating that program.
	GLuint m_prog;

	// Locations of "uniform variables" which known as "cbuffer registers" in HLSL.
	GLint world, view, proj, shaderTexture, noiseParameters;

public:
	CNoiseProgram(): m_prog( 0 ) {}
	~CNoiseProgram()
	{
		destroy();
	}

	// Load, compile and link the shaders
	HRESULT initialize();

	void destroy();

	// Set the texture sampler constant
	void setTexture( int nTextureUnit );

	// Set word, view and projection matrices
	void setTransforms( const Matrix& worldMatrix, const Matrix& viewMatrix, const Matrix& projectionMatrix ) const;

	// Set noise-related shader constants
	void setNoiseParameters( Vector2 noiseXyScale, float noiseResultScale, float time ) const;

	void use() const
	{
		glUseProgram( m_prog );
	}
};