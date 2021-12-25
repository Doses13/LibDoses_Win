template<class USER_CLASS>
HRESULT D2DWindow<USER_CLASS>::MKEventSystem(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MKEvent e;
	object* obj = nullptr;

	switch (uMsg)
	{
	case WM_MOUSEMOVE:
	{
		InvalidateRect(m_hwnd, NULL, FALSE);			// tells the window to redraw
		e.cords.x = GET_X_LPARAM(lParam);
		e.cords.y = GET_Y_LPARAM(lParam);
		obj = m_con.getObjAtCord(e.cords.x, e.cords.y);	// gets what object is under the pointer now
		DC << L"Current: " << (int)obj << L"\n" << L"Last: " << (int)m_lastObject << L"\n";
		DC << e.cords.x << L", " << e.cords.y << L"\n";
		if (obj)
		{
			if (obj != m_lastObject)						// if mouse is on a new object
			{
				e.button = mouseEvent::hoverOn;
				obj->EventFunction(e);						// Runs hoverOn for new object
				if (m_lastObject)
				{
					e.button = mouseEvent::hoverOff;
					m_lastObject->EventFunction(e);			// runs hoverOff for last object
				}
			}
		}
		else if (m_lastObject)
		{
			e.button = mouseEvent::hoverOff;
			m_lastObject->EventFunction(e);
		}
		m_lastObject = obj;
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		e.cords.x = GET_X_LPARAM(lParam);
		e.cords.y = GET_Y_LPARAM(lParam);
		obj = m_con.getObjAtCord(e.cords.x, e.cords.y);
		DC << L"Current: " << (int)obj << L"\n" << L"Last: " << (int)m_lastObject << L"\n";
		DC << e.cords.x << L", " << e.cords.y << L"\n";
		e.button = mouseEvent::leftDown;
		if (obj)
		{
			InvalidateRect(m_hwnd, NULL, FALSE);
			return obj->EventFunction(e);
		}
		else return 0;
	}
	case WM_LBUTTONUP:
	{
		e.cords.x = GET_X_LPARAM(lParam);
		e.cords.y = GET_Y_LPARAM(lParam);
		obj = m_con.getObjAtCord(e.cords.x, e.cords.y);
		DC << L"Current: " << (int)obj << L"\n" << L"Last: " << (int)m_lastObject << L"\n";
		DC << e.cords.x << L", " << e.cords.y << L"\n";
		e.button = mouseEvent::leftUp;
		if (obj) 
		{ 
			InvalidateRect(m_hwnd, NULL, FALSE);
			return obj->EventFunction(e); 
		}
		else return 0;
	}
	default:
		break;
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}