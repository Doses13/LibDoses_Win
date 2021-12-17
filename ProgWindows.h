#pragma once
#include "LibDosesWin.h"

class MainWindow : public D2DWindow<MainWindow>
{
	HRESULT CreateGraphicsResources() override;
	void OnPaint() override;

public:

	MainWindow() {};
};

#include "ProgWindows.cpp"