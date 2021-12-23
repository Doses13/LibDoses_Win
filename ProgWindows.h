#pragma once
#include "LibDosesWin.h"
#include "ProgObjects.h"

using namespace D2D1;

class MainWindow : public D2DWindow<MainWindow>
{
	HRESULT CreateGraphicsResources() override;
	HRESULT OnPaint() override;

	button1 b1;
	button1 b2;
	button1 b3;
	button1 b4;

public:
	MainWindow() : b1(100,100), b2(155,100), b3(210,100), b4(265,100)
	{	
		
	};
};

#include "ProgWindows.cpp"