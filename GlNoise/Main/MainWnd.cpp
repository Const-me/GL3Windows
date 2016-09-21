#include "stdafx.h"
#include "MainWnd.h"
#include <resource.h>

using namespace DirectX;

void CMainWnd::resetParameters()
{
	totalTime = 0;
	noiseXyScale = Vector2( 30, 30 );
	noiseResultScale = 0.03f;
	timeScaleFactor = 1;

	world = worldInitial = Matrix::Identity;
}

HRESULT CMainWnd::create( int nCmdShow )
{
	HWND res = __super::Create( NULL, NULL, L"GlNoise demo" );
	if( NULL == res )
		return GetLastHr();

	ShowWindow( nCmdShow );
	return S_OK;
}

HRESULT CMainWnd::createResources( CString& failReason )
{
	const int nGeosphereDivisions = 4;

#define CHECK_REASON( hr, reason )                           \
{                                                            \
	const HRESULT _hr = ( hr );                              \
	if( FAILED( _hr ) )                                      \
	{                                                        \
		AtlTrace( "%s (%s, line %i): %S",                    \
				__func__, __FILE__, (int)__LINE__, (const wchar_t*)( ErrorMessage( _hr ) ) );    \
		failReason = reason;                                 \
		return _hr;                                          \
	}                                                        \
}

	// Create a geosphere mesh
	CHECK_REASON( m_mesh.create( nGeosphereDivisions ), L"Unable to create the mesh." );

	// Upload the mesh to GPU
	CHECK_REASON( m_mesh.createBuffers(), L"Unable to create VBO / IBO." );

	// Load the texture
	CHECK_REASON( m_tex.loadCompressedDdsResource( 0, IDR_DDS1, L"dds" ), L"Unable to load the texture." );

	// Load the shaders
	CHECK_REASON( m_prog.initialize(), L"Unable to compile or link the shaders." );

#undef CHECK_REASON

	return S_OK;
}

void CMainWnd::destroyResources()
{
	m_prog.destroy();
	m_mesh.destroyBuffers();
	m_tex.destroy();
}

void CMainWnd::initScene( CSize viewportSize )
{
	const float degFovY = 45;
	const float zNear = 0.1f;
	const float zFar = 7;

	const float radFovY = XMConvertToRadians( degFovY );
	const float w2h = float( viewportSize.cx ) / float( viewportSize.cy );
	proj = XMMatrixPerspectiveFovRH( radFovY, w2h, zNear, zFar );
	view = XMMatrixLookAtRH( Vector3( -3, -1, 0 ), Vector3::Zero, Vector3::UnitZ );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	m_tex.bind();
	m_prog.use();
}

void CMainWnd::drawScene( float SecsElapsed )
{
	// Update time variable
	totalTime += ( SecsElapsed * timeScaleFactor );

	// Update shader variables
	m_prog.setShaderParameters( world, view, proj, noiseXyScale, noiseResultScale, float( totalTime ) );

	// Draw the mesh. All the data should be already on the GPU.
	m_mesh.drawBuffers();
}

void CMainWnd::dragStart()
{
	worldInitial = world;
}

void CMainWnd::dragDelta( const Vector2& amount )
{
	const float rotationMul = XM_PI;
	const Vector2 rotationAngles = amount * rotationMul;

	Matrix rel = Matrix::CreateFromYawPitchRoll( -rotationAngles.y, 0, rotationAngles.x );
	world = worldInitial * rel;
}

LRESULT CMainWnd::OnWmKeyDown( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	const double timeScaleStep = 1.1;
	const float xyScaleStep = 1.5f;
	const float resultScaleStep = 1.2f;

	switch( wParam )
	{
	case VK_RIGHT:
	case VK_NUMPAD6:
		timeScaleFactor *= timeScaleStep;
		return 0;
	case VK_LEFT:
	case VK_NUMPAD4:
		timeScaleFactor /= timeScaleStep;
		return 0;
	case VK_UP:
	case VK_NUMPAD8:
		noiseXyScale *= xyScaleStep;
		return 0;
	case VK_DOWN:
	case VK_NUMPAD2:
		noiseXyScale /= xyScaleStep;
		return 0;
	case VK_ADD:
		noiseResultScale *= resultScaleStep;
		return 0;
	case VK_SUBTRACT:
		noiseResultScale /= resultScaleStep;
		return 0;
	case VK_RETURN:
	case VK_NUMPAD5:
		resetParameters();
		return 0;
	}
	bHandled = FALSE;
	return 0;
}