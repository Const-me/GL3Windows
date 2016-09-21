#include "stdafx.h"
#include "SphereMesh.h"

void CSphereMesh::makeIcosahedron()
{
	// http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html

	// Calculate normalized coordinate values
	const double T = ( 1.0 + sqrt( 5.0 ) ) / 2.0;
	const double mul = 1.0 / sqrt( T * T + 1.0 * 1.0 );
	const float t = float( T * mul );
	const float l = float( 1.0 * mul );

	// Add vertices using those values

	addVertex( -l, +t, 0 );	// 0
	addVertex( +l, +t, 0 );	// 1
	addVertex( -l, -t, 0 );	// 2
	addVertex( +l, -t, 0 );	// 3

	addVertex( 0, -l, +t );	// 4
	addVertex( 0, +l, +t );	// 5
	addVertex( 0, -l, -t );	// 6
	addVertex( 0, +l, -t );	// 7

	addVertex( +t, 0, -l );	// 8
	addVertex( +t, 0, +l );	// 9
	addVertex( -t, 0, -l );	// 10
	addVertex( -t, 0, +l );	// 11

	// 5 faces around point 0
	addTriangle( 0, 11, 5 );
	addTriangle( 0, 5, 1 );
	addTriangle( 0, 1, 7 );
	addTriangle( 0, 7, 10 );
	addTriangle( 0, 10, 11 );

	// 5 adjacent faces
	addTriangle( 1, 5, 9 );
	addTriangle( 5, 11, 4 );
	addTriangle( 11, 10, 2 );
	addTriangle( 10, 7, 6 );
	addTriangle( 7, 1, 8 );

	// 5 faces around point 3
	addTriangle( 3, 9, 4 );
	addTriangle( 3, 4, 2 );
	addTriangle( 3, 2, 6 );
	addTriangle( 3, 6, 8 );
	addTriangle( 3, 8, 9 );

	// 5 adjacent faces
	addTriangle( 4, 9, 5 );
	addTriangle( 2, 4, 11 );
	addTriangle( 6, 2, 10 );
	addTriangle( 8, 6, 7 );
	addTriangle( 9, 8, 1 );
}

// Make edge key from two integer vertices
static inline QWORD edgeKey( int a, int b )
{
	union U
	{
		QWORD key;
		struct
		{
			int a;
			int b;
		}
		ab;
	}
	res;
	// Sort them
	if( a <= b )
	{
		res.ab.a = a;
		res.ab.b = b;
	}
	else
	{
		res.ab.a = b;
		res.ab.b = a;
	}
	return res.key;
}

void CSphereMesh::divideOnce()
{
	std::unordered_map<QWORD, int> edgesSplit;
	// Swap this->triangles with a new vector
	std::vector<triangleIndices> tris;
	tris.swap( triangles );
	triangles.reserve( tris.size() * 4 );

	vertices.reserve( vertices.size() + triangles.size() * 3 / 2 );

	for( const triangleIndices& src : tris )
	{
		std::array<int, 3> mid;

		// Append 3 mid.points to the vertex buffer, recording their indices in mid array.
		forEachTriangleEdge( src, [ this, &edgesSplit, &mid ] ( const int nEdge, int i1, int i2 )
		{
			const QWORD op = edgeKey( i1, i2 );
			auto it = edgesSplit.find( op );
			if( it != edgesSplit.end() )
			{
				// Already split this edge: use cached vertex ID
				mid[ nEdge ] = it->second;
				return;
			}

			const Vector3 v1 = vertices[ i1 ].position;
			const Vector3 v2 = vertices[ i2 ].position;
			Vector3 vMid = ( v1 + v2 ) * 0.5;
			vMid.Normalize();

			const int newId = int( vertices.size() );
			mid[ nEdge ] = newId;
			edgesSplit[ op ] = newId;
			vertices.push_back( vMid );
		} );

		// Insert 4 new triangles in the index buffer.

		// src[0]
		//   |       \
		// mid[2]        mid[0]
		//   |                     \
		// src[2]   --   mid[1]   --   src[1]

		addTriangle( src[ 0 ], mid[ 0 ], mid[ 2 ] );
		addTriangle( src[ 1 ], mid[ 1 ], mid[ 0 ] );
		addTriangle( src[ 2 ], mid[ 2 ], mid[ 1 ] );
		addTriangle( mid[ 0 ], mid[ 1 ], mid[ 2 ] );
	}
}

HRESULT CSphereMesh::create( int nDivisions )
{
	if( nDivisions < 0 ) return E_INVALIDARG;

	makeIcosahedron();

	for( int i = 0; i < nDivisions; i++ )
		divideOnce();

	// Calculate UV
	for( auto& v : vertices )
	{
		const float yRad = asinf( v.position.z );	// radians, from -pi/2 to +pi/2
		v.texCoord.y = ( v.position.z + DirectX::XM_PIDIV2 ) / DirectX::XM_PI;

		const float xRad = atan2f( v.position.y, v.position.x ) + DirectX::XM_PI;	// Radians, from 0 to 2Pi
		float x = xRad / DirectX::XM_PI;

		// Wrap manually, to have nicer transition.
		x = std::clamp<float>( x, 0, 2 );
		if( x > 1 )
			x = 2.0f - x;
		v.texCoord.x = x;
	}

	// debug code below
	if( false )
	{
		Vector2 i( FLT_MAX, FLT_MAX );
		Vector2 a = -i;
		for( const auto& v : vertices )
		{
			i = Vector2::Min( i, v.texCoord );
			a = Vector2::Max( a, v.texCoord );
		}
		CStringA msg;
		msg.Format( "UV: {%f, %f} - {%f, %f}\n", i.x, i.y, a.x, a.y );
		OutputDebugStringA( msg );
	}

	return S_OK;
}