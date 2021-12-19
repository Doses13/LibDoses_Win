#pragma once
#include "LibDosesWin.h"

class MainWindow : public D2DWindow<MainWindow>
{
	HRESULT CreateGraphicsResources() override;
	void OnPaint() override;

	// Colors
	D2D1_COLOR_F c_cyan;


	// Brushes
	ID2D1SolidColorBrush* b1 = NULL;
	ID2D1SolidColorBrush* b2 = NULL;

	// Shapes
	D2D1_RECT_F r1;

	// Text
	IDWriteTextFormat* wtf1;

public:

	MainWindow()
	{
		r1.top = 50;
		r1.bottom = 100;
		r1.left = 50;
		r1.right = 200;
	};
};

#include "ProgWindows.cpp"