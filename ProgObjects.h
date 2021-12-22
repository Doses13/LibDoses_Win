#pragma once

// changes color when clicked on
class button1 : public rect
{
	bool m_active = 0;
	void swap();

	unsigned m_offColor = 0x373737;
	unsigned m_onColor  = 0x7f3737;

	ID2D1SolidColorBrush* m_sb;

public:
	button1();
	HRESULT EventFunction(MKEvent Event) override;
};

#include "ProgObjects.cpp"