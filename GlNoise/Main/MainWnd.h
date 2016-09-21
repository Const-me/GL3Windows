#pragma once
#include <Window/GlWnd.h>
#include <Shaders/NoiseProgram.h>
#include <Mesh/SphereMesh.h>
#include <Texture/Texture.h>
#include "MouseProcessor.h"

// The main window class that integrates other parts of the app.
class CMainWnd:
	public CGlWnd,
	public CMouseProcessor
{
public:
	CMainWnd()
	{
		resetParameters();
	}
	~CMainWnd() { }

	HRESULT create( int nCmdShow );

	BEGIN_MSG_MAP( CMainWnd )
		CHAIN_MSG_MAP( CGlWnd )
		CHAIN_MSG_MAP( CMouseProcessor )
		MESSAGE_HANDLER( WM_KEYDOWN, OnWmKeyDown )
	END_MSG_MAP()

private:

	void OnFinalMessage( HWND wnd ) override
	{
		// Gracefully exit the application when this window is destroyed.
		PostQuitMessage( 0 );
	}

	// CGlWnd virtual methods

	HRESULT createResources( CString& failReason ) override;

	void destroyResources() override;

	void initScene( CSize viewportSize ) override;

	void drawScene( float SecsElapsed ) override;

	// Some GPU resources
	CSphereMesh m_mesh;
	CNoiseProgram m_prog;
	CTexture m_tex;

	// Camera
	Matrix world, view, proj;

	// Noise-related things
	void resetParameters();
	Vector2 noiseXyScale;
	float noiseResultScale;
	double totalTime;
	double timeScaleFactor;

	// Mouse rotations, CMouseProcessor inherited virtual methods..
	Matrix worldInitial;
	void dragStart() override;
	void dragDelta( const Vector2& amount ) override;

	// Keyboard input
	LRESULT OnWmKeyDown( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
};