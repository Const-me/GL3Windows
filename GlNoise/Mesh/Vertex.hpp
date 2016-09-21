#pragma once
#include <Utils/dxtk/SimpleMath.h>

// Vertex data
#pragma pack (4)
struct Vertex
{
	Vector3 position;
	Vector2 texCoord;

	Vertex( Vector3 pos ) :
		position( pos ), texCoord( Vector2::Zero ) { }

	static HRESULT initializeFormat( GLuint vertexBufferId )
	{
		// Enable the two vertex array attributes
		glEnableVertexAttribArray( 0 );  // position
		glEnableVertexAttribArray( 1 );  // texCoord

		// Specify the location and format of the position portion of the vertex buffer.
		glBindBuffer( GL_ARRAY_BUFFER, vertexBufferId );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), 0 );

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		glBindBuffer( GL_ARRAY_BUFFER, vertexBufferId );
		glVertexAttribPointer( 1, 2, GL_FLOAT, false, sizeof( Vertex ), (unsigned char*)NULL + ( 3 * sizeof( float ) ) );

		return getLastGl();
	}
};