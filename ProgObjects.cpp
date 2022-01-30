// Button1

dynArray<button1*> button1::sm_allButtons;
bool button1::sm_onlyOne = 1;

HRESULT button1::EventFunction(MKEvent Event)
{
	switch (Event.button)
	{
	case mouseEvent::leftUp:
		getContainer()->setCapture(nullptr);
		if (sm_onlyOne)
		{
			turnOffAllOthers(this);
		}
		if (m_depressed)
		{
			
			move(0, -2);
			swap();
			m_depressed = 0;
		}
		m_dragging = 0;
		break;
	case mouseEvent::leftDown:
		getContainer()->setCapture(this);
		m_dragStart = Event.cords;
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
		m_dragging = 0;
		break;
	case mouseEvent::move:
		if (m_depressed && distance(Event.cords, m_dragStart) > m_dragThreshhold)
		{
			// hover off
			MKEvent t;
			t.button = mouseEvent::hoverOff;
			EventFunction(t);

			m_dragging = 1;
		}
		if (m_dragging)
		{
			setPos(Event.cords.x, Event.cords.y);
		}
	default:
		break;
	}
	return S_OK;
}
void button1::swap()
{
	if (m_active) // now make button in-active
	{
		getSolidBrush()->setColor(m_offHoverColor);
		m_active = false;
	}
	else // make button active
	{
		getSolidBrush()->setColor(m_onHoverColor);
		m_active = true;
	}
}
void button1::turnOffAllOthers(button1* exception)
{
	for (int i = 0; i < sm_allButtons.size(); i++)
	{
		if (sm_allButtons[i] != exception)
		{
			sm_allButtons[i]->setStatus(false);
		}
	}
}
void button1::setStatus(bool set)
{
	if (m_active && !set)		// will turn the object off
	{
		getSolidBrush()->setColor(m_offColor);
		m_active = 0;
	}
	else if (!m_active && set)	// will turn the object on
	{
		getSolidBrush()->setColor(m_onColor);
		m_active = 1;
	}
}

button1::button1(float X, float Y) : rect(X,Y,50,40)
{
	getSolidBrush()->setColor(m_offColor);
	sm_allButtons.push_back(this);
}