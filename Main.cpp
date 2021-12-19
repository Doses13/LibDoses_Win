#include "LibDosesWin.h"
#include "ProgWindows.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	MainWindow win0;
    if (!win0.Create(L"Window", WS_OVERLAPPEDWINDOW)) { return 0; }
    ShowWindow(win0.Handle(), nCmdShow);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	return 0;
}