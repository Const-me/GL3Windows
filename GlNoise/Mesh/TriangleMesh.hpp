#pragma once

// Triangle mesh with arbitrary vertex format, and uint32_t indices.
// The vertex class must expose following static method to describe its memory layout to OpenGL: HRESULT initializeFormat( GLuint vertexBufferId )
template<class tVertex>
class CTriangleMesh
{
protected:
	std::vector<tVertex> vertices;

	typedef std::array<int, 3> triangleIndices;
	std::vector<triangleIndices> triangles;

	inline void addTriangle( int a, int b, int c )
	{
		triangles.emplace_back( triangleIndices { a, b, c } );
	}

private:
	// Vertex Array Object (VAO), client side state, i.e. VBO bindings, IBO bindings and vertex layout.
	mutable GLuint m_array;

	// Vertex Buffer Object (VBO), vertex buffer on the GPU.
	mutable GLuint m_vb;

	// Index Buffer Object (IBO), index buffer on the GPU.
	mutable GLuint m_ib;

	HRESULT createVertexBuffer() const
	{
		GLuint vbo;
		glGenBuffers( 1, &vbo );
		CHECK( getLastGl() );
		
		// Allocate space and upload the data from CPU to GPU
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		CHECK( getLastGl() );

		glBufferData( GL_ARRAY_BUFFER, sizeof( tVertex ) * vertices.size(), vertices.data(), GL_STATIC_DRAW );
		CHECK( getLastGl() );

		m_vb = vbo;
		return S_OK;
	}

	HRESULT createIndexBuffer() const
	{
		GLuint vbo;
		glGenBuffers( 1, &vbo );
		CHECK( getLastGl() );

		// Allocate space and upload the data from CPU to GPU
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo );
		CHECK( getLastGl() );

		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( triangleIndices ) * triangles.size(), triangles.data(), GL_STATIC_DRAW );
		CHECK( getLastGl() );

		m_ib = vbo;
		return S_OK;
	}

public:

	// Create those OpenGL objects
	HRESULT createBuffers() const
	{
		// Create the VAO
		glGenVertexArrays( 1, &m_array );

		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		glBindVertexArray( m_array );

		// Create VBO, fill with data
		CHECK( createVertexBuffer() );

		// Describe layout, will be stored in the VAO
		CHECK( tVertex::initializeFormat( m_vb ) );

		// Create IBO, fill with data
		CHECK( createIndexBuffer() );

		return S_OK;
	}

	// Draw from those VBOs
	bool drawBuffers() const
	{
		if( !glIsBuffer( m_vb ) )
			return false;
		if( !glIsBuffer( m_ib ) )
			return false;

		glBindVertexArray( m_array );

		glDrawElements( GL_TRIANGLES, (GLsizei)( triangles.size() * 3 ), GL_UNSIGNED_INT, 0 );

		return true;
	}

	CTriangleMesh() : m_array( 0 ), m_ib( 0 ), m_vb( 0 ) { }
	virtual ~CTriangleMesh()
	{
		destroyBuffers();
	}

	void destroyBuffers()
	{
		if( glIsBuffer( m_vb ) )
		{
			glDeleteBuffers( 1, &m_vb );
			m_vb = 0;
		}
		if( glIsBuffer( m_ib ) )
		{
			glDeleteBuffers( 1, &m_ib );
			m_ib = 0;
		}

		if( 0 != m_array )
		{
			glBindVertexArray( 0 );
			glDeleteVertexArrays( 1, &m_array );
			m_array = 0;
		}
	}
};