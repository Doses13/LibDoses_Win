void MainWindow::OnPaint()
{
	pRenderTarget->FillRectangle(r1, b1);
	pRenderTarget->DrawTextW(L"Joseph\nIs Awesome", 17, wtf1, r1, b2);
}

HRESULT MainWindow::CreateGraphicsResources()
{
	HRESULT hr = S_OK;

	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF::ColorF(D2D1::ColorF::Pink), &b1);
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF::ColorF(D2D1::ColorF::Black), &b2);
	pDWriteFactory_->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18, L"Arial", &wtf1);

	return hr;
}