#include "stdafx.h"
#include "MouseProcessor.h"

LRESULT CMouseProcessor::OnWmMouseButton( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SetCapture( m_WindowHandle );
	m_bDragging = true;
	m_downPoint = CPoint( lParam );

	CRect rc;
	GetClientRect( m_WindowHandle, &rc );
	m_scaleMul = 1.0f / rc.Height();

	dragStart();
	return 0;
}

LRESULT CMouseProcessor::OnWmMouseLeave( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ReleaseCapture();
	m_bDragging = false;
	return 0;
}

LRESULT CMouseProcessor::OnWmMouseMove( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( !m_bDragging )
		return 0;

	const CPoint pt( lParam );
	Vector2 delta( float( pt.x - m_downPoint.x ), float( pt.y - m_downPoint.y ) );
	delta *= m_scaleMul;
	dragDelta( delta );
	return 0;
}