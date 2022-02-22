#pragma once
#include "LibDosesWin.h"
#include "ProgObjects.h"

using namespace D2D1;

class MainWindow : public D2DWindow<MainWindow>
{
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	HRESULT CreateGraphicsResources() override;
	void CalculateLayout(D2D1_SIZE_U size) override;
	HRESULT OnPaint() override;

	button1 b1;
	button1 b2;
	button1 b3;
	button1 b4;

public:
	MainWindow() : b1(100,100), b2(160,100), b3(220,100), b4(280,100)
	{	
		
	};
};

#include "ProgWindows.cpp"