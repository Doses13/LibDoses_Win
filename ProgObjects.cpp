// Button1
HRESULT button1::EventFunction(MKEvent Event)
{
	switch (Event.button)
	{
	case mouseEvent::leftUp:
		if (m_depressed)
		{
			move(0, -2);
			swap();
			m_depressed = 0;
		}
		break;
	case mouseEvent::leftDown:
		getSolidBrush()->setColor(m_depressedColor);
		move(0, 2);
		m_depressed = 1;
		break;
	case mouseEvent::hoverOn:
		if (m_active)
		{
			getSolidBrush()->setColor(m_onHoverColor);
		}
		else
		{
			getSolidBrush()->setColor(m_offHoverColor);
		}
		break;
	case mouseEvent::hoverOff:
		if (m_active)
		{
			getSolidBrush()->setColor(m_onColor);
		}
		else
		{
			getSolidBrush()->setColor(m_offColor);
		}
		if (m_depressed)
		{
			move(0, -2);
			m_depressed = 0;
		}
		break;
	default:
		break;
	}
	return S_OK;
}
void button1::swap()
{
	if (m_active) // now make button in-active
	{
		getSolidBrush()->setColor(m_offColor);
		m_active = false;
	}
	else // make button active
	{
		getSolidBrush()->setColor(m_onColor);
		m_active = true;
	}
}
button1::button1(float X, float Y) : rect(X,Y,50,40)
{
	getSolidBrush()->setColor(m_offColor);
}