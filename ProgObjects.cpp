HRESULT button1::EventFunction(MKEvent Event)
{
	if (Event.button == mouseEvent::leftUp)
	{
		swap();
	}
	return S_OK;
}

void button1::swap()
{
	if (m_active) // now make button in-active
	{
		m_sb->SetColor(D2D1::ColorF(m_offColor));
		m_active = false;
	}
	else // make button active
	{
		m_sb->SetColor(D2D1::ColorF(m_onColor));
		m_active = true;
	}
}

button1::button1()
{
	m_sb = reinterpret_cast<ID2D1SolidColorBrush*>(getBrush()->getPtr());
	m_sb->SetColor(D2D1::ColorF(m_offColor));
}