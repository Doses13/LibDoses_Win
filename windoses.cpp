template<class USER_CLASS>
HRESULT D2DWindow<USER_CLASS>::MKEventSystem(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MKEvent e;
	e.cords.x = GET_X_LPARAM(lParam);
	e.cords.y = GET_Y_LPARAM(lParam);

	object* obj = m_con.getObjAtCord(e.cords.x, e.cords.y);

	DC.log(L"0");

	if (obj)
	{
		DC.log(L"1");
		InvalidateRect(m_hwnd, NULL, FALSE);
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			if (obj != m_lastObject) // if mouse is on a new object
			{
				e.button = mouseEvent::hoverOn;
				obj->EventFunction(e);
				if (m_lastObject)
				{
					e.button = mouseEvent::hoverOff;
					m_lastObject->EventFunction(e);
				}
				m_lastObject = obj;
			}
			return 0;
		case WM_LBUTTONDOWN:
			e.button = mouseEvent::leftDown;
			return obj->EventFunction(e);
		case WM_LBUTTONUP:
			e.button = mouseEvent::leftUp;
			return obj->EventFunction(e);
		default:
			DC.log(L"");
			break;
		}
	}
	else if (m_lastObject) // if hover off an object into the void
	{
		DC.log(L"2");
		InvalidateRect(m_hwnd, NULL, FALSE);
		e.button = mouseEvent::hoverOff;
		m_lastObject->EventFunction(e);
		m_lastObject = nullptr;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}