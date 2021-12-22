HRESULT MainWindow::OnPaint()
{
	HRESULT hr = S_OK;

	m_con.render();

	return hr;
}

HRESULT MainWindow::CreateGraphicsResources()
{
	HRESULT hr = S_OK;

	/*
	pDWriteFactory_->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 36, L"Arial", &wtf1);
	*/
	return hr;
}