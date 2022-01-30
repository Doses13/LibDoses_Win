#pragma once

// changes color when clicked on
class button1 : public rect
{
	bool m_active = 0;
	bool m_depressed = 0;
	bool m_dragging = 0;
	void swap();
	void turnOffAllOthers(button1* exception);

	unsigned m_offColor = 0x7f7f7f;
	unsigned m_onColor  = 0xcf3737;
	unsigned m_depressedColor = 0x5f5f5f;
	unsigned m_offHoverColor = 0x9f9f9f;
	unsigned m_onHoverColor = 0xaf3737;
	
	D2D1_POINT_2L m_dragStart;
	float m_dragThreshhold = 8;

	static dynArray<button1*> sm_allButtons;
	static bool sm_onlyOne;

public:
	button1(float X, float Y);

	void setStatus(bool set);
	bool getStatus();

	HRESULT EventFunction(MKEvent Event) override;
};
#include "ProgObjects.cpp"