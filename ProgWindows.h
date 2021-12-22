#pragma once
#include "LibDosesWin.h"
#include "ProgObjects.h"

using namespace D2D1;

class MainWindow : public D2DWindow<MainWindow>
{
	HRESULT CreateGraphicsResources() override;
	HRESULT OnPaint() override;

	button1 test;

public:
	MainWindow() 
	{	
		
	};
};

#include "ProgWindows.cpp"