#pragma once

// This uitility class translates mouse drag windows messages into rotation-friendly events
class CMouseProcessor
{
public:
	CMouseProcessor()
	{
		m_bDragging = false;
	}
	~CMouseProcessor() { }

private:
	BEGIN_MSG_MAP( CMouseProcessor )
		m_WindowHandle = hWnd;

		MESSAGE_HANDLER( WM_LBUTTONDOWN, OnWmMouseButton )

		MESSAGE_HANDLER( WM_MOUSELEAVE, OnWmMouseLeave )
		MESSAGE_HANDLER( WM_LBUTTONUP, OnWmMouseLeave )

		MESSAGE_HANDLER( WM_MOUSEMOVE, OnWmMouseMove )
	END_MSG_MAP()

	// Mouse WM handlers
	LRESULT OnWmMouseButton( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnWmMouseMove( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnWmMouseLeave( UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	HWND m_WindowHandle;
	bool m_bDragging;
	float m_scaleMul;
	CPoint m_downPoint;

protected:
	// Called when the left mouse is down
	virtual void dragStart() = 0;

	// While dragging, receives the relative mouse position change since when the dragStart was called.
	// The method argument is Y-normalized, i.e. moving mouse the whole window's height will result in amount.y = 1 or -1.
	virtual void dragDelta( const Vector2& amount ) = 0;
};