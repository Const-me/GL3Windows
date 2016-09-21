#version 130 // OpenGL 3.0

// INPUT VARIABLES

in vec3 inputPosition;
in vec2 inputTexcoord;

// OUTPUT VARIABLES

out vec2 texCoord;

// UNIFORM VARIABLES

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Vertex Shader

void main(void)
{
	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = worldMatrix * vec4( inputPosition, 1.0f );
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	// Store the texture coordinates for the pixel shader
	texCoord = inputTexcoord;
}