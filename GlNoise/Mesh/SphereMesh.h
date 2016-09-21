#pragma once
#include "Vertex.hpp"
#include "TriangleMesh.hpp"

// This class holds the geosphere mesh
class CSphereMesh: public CTriangleMesh<Vertex>
{
public:
	// Create the geosphere with the specified LoD
	HRESULT create( int nDivisions );

private:
	inline void addVertex( float x, float y, float z )
	{
		vertices.emplace_back( Vector3( x, y, z ) );
	}

	// Create icosahedron mesh
	void makeIcosahedron();

	// Divide each edge in 2, each face in 4
	void divideOnce();

	// Invoke the action once for each edge of the triangle. The action must accept 3 integer arguments: edge # (from 0 to 3), first vertex index, second vertex index.
	template<class Action>
	static inline void forEachTriangleEdge( const triangleIndices& tri, Action& act )
	{
		act( 0, tri[ 0 ], tri[ 1 ] );
		act( 1, tri[ 1 ], tri[ 2 ] );
		act( 2, tri[ 2 ], tri[ 0 ] );
	}
};