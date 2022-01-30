template<class USER_CLASS>
HRESULT D2DWindow<USER_CLASS>::MKEventSystem(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MKEvent e;
	object* obj = nullptr;

	//DC << uMsg << nll;

	switch (uMsg)
	{
	case WM_MOUSEMOVE:
	{
		e.cords.x = GET_X_LPARAM(lParam);
		e.cords.y = GET_Y_LPARAM(lParam);
		if (m_con.getCapture())
		{
			InvalidateRect(m_hwnd, NULL, FALSE);			// tells the window to redraw
			e.button = mouseEvent::move;
			m_con.getCapture()->EventFunction(e);
		}
		else
		{
			obj = m_con.getObjAtCord(e.cords.x, e.cords.y);	// gets what object is under the pointer now
			if (obj)
			{
				InvalidateRect(m_hwnd, NULL, FALSE);			// tells the window to redraw
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
				else	// Mouse moved on the same object
				{
					e.button = mouseEvent::move;
					obj->EventFunction(e);
				}
			}
			else if (m_lastObject)
			{
				InvalidateRect(m_hwnd, NULL, FALSE);			// tells the window to redraw
				e.button = mouseEvent::hoverOff;
				m_lastObject->EventFunction(e);
			}
		}

		//DC << L"Current: " << (int)obj << L"\n" << L"Last: " << (int)m_lastObject << L"\n";
		//DC << e.cords.x << L", " << e.cords.y << L"\n";

		m_lastObject = obj;
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		e.cords.x = GET_X_LPARAM(lParam);
		e.cords.y = GET_Y_LPARAM(lParam);
		obj = m_con.getObjAtCord(e.cords.x, e.cords.y);
		//DC << L"Current: " << (int)obj << L"\n" << L"Last: " << (int)m_lastObject << L"\n";
		//DC << e.cords.x << L", " << e.cords.y << L"\n";
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
		//DC << L"Current: " << (int)obj << L"\n" << L"Last: " << (int)m_lastObject << L"\n";
		//DC << e.cords.x << L", " << e.cords.y << L"\n";
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