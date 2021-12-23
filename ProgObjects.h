#pragma once

// changes color when clicked on
class button1 : public rect
{
	bool m_active = 0;
	bool m_depressed = 0;
	void swap();

	unsigned m_offColor = 0x373737;
	unsigned m_onColor  = 0x7f3737;
	unsigned m_depressedColor = 0x232323;
	unsigned m_offHoverColor = 0x474747;
	unsigned m_onHoverColor = 0x8f3737;

public:
	button1(float X, float Y);
	HRESULT EventFunction(MKEvent Event) override;
};

#include "ProgObjects.cpp"