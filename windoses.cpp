template<class USER_CLASS>
HRESULT D2DWindow<USER_CLASS>::MKEventSystem(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MKEvent e;
	e.cords.x = GET_X_LPARAM(lParam);
	e.cords.y = GET_Y_LPARAM(lParam);

	object* obj = m_con.getObjAtCord(e.cords.x, e.cords.y);

	if (obj)
	{
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			// if new object:
				// hoverOn on new object
				// hoverOff on old object

			return 0;
		case WM_LBUTTONDOWN:
			e.button = mouseEvent::leftUp;
			return obj->EventFunction(e);
		case WM_LBUTTONUP:
			e.button = mouseEvent::leftDown;
			return obj->EventFunction(e);

		default:
			break;
		}
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}