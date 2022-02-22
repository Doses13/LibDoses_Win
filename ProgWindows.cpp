#include "ProgWindows.h"
HRESULT MainWindow::OnPaint()
{
	HRESULT hr = S_OK;

	m_con.render();

	return hr;
}

HRESULT MainWindow::CreateGraphicsResources()
{
	HRESULT hr = S_OK;

	return hr;
}

void MainWindow::CalculateLayout(D2D1_SIZE_U size)
{

	DC << size.height << L", " << size.width << L"\n";

}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}