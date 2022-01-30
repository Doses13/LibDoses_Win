// this file makes the creation and manegment of windows easier
#pragma once

#include "LibDosesWin.h"
#include <string>
#include <iostream>

#define nll L"\n"

// RANDOM THINGS

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
};
class DPIScale
{
    static float scaleX;
    static float scaleY;

public:
    static void Initialize(HWND hwnd)
    {
        FLOAT dpi = GetDpiForWindow(hwnd);
        //pFactory->GetDesktopDpi(&dpiX, &dpiY);
        scaleX = dpi / 96.0f;
        scaleY = dpi / 96.0f;
    }

    template <typename T>
    static D2D1_POINT_2F PixelsToDips(T x, T y)
    {
        return D2D1::Point2F(static_cast<float>(x) / scaleX, static_cast<float>(y) / scaleY);
    }
};
float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;

static class DEBUG_CONSOLE_D2D
{
    friend DEBUG_CONSOLE_D2D& operator<<(DEBUG_CONSOLE_D2D& dc, const wchar_t* rhs);
    friend DEBUG_CONSOLE_D2D& operator<<(DEBUG_CONSOLE_D2D& dc, const int& rhs);

    ID2D1HwndRenderTarget* m_pRT = nullptr;
    IDWriteFactory* m_pWF = nullptr;

    IDWriteTextFormat* m_WTF = nullptr;
    ID2D1SolidColorBrush* m_pSB = nullptr;

    D2D1_RECT_F m_rect;
    std::wstring m_text;
    bool m_displayLog = 1;
    bool m_acceptNew = 1;

public:
    HRESULT create(ID2D1HwndRenderTarget* pRT, IDWriteFactory* pWF)
    {
        HRESULT hr = S_OK;
        m_pRT = pRT;
        m_pWF = pWF;
        hr = m_pWF->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12, L"Arial", &m_WTF);
        hr = m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGreen), &m_pSB);
        m_rect.right = 200;
        return hr;
    }
    void startLogging();
    void stopLogging();
    void clearLog();
    const wchar_t* getLog()
    {
        return m_text.c_str();
    }
    void display()
    {
        m_pRT->DrawTextW(m_text.c_str(), m_text.size(), m_WTF, m_rect, m_pSB);
        m_text.clear();
    }
}DC;

// overloads of << for debug log
DEBUG_CONSOLE_D2D& operator<<(DEBUG_CONSOLE_D2D& dc, const wchar_t* rhs)
{
    dc.m_text.append(rhs);
    return dc;
}

DEBUG_CONSOLE_D2D& operator<<(DEBUG_CONSOLE_D2D& dc, const int& rhs)
{
    dc.m_text.append(std::to_wstring(rhs));
    return dc;
}

// BASE WINDOW CLASSES

template <class DERIVED_TYPE>
class BaseWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE* pThis = NULL;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : m_hwnd(NULL) { }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle = WS_OVERLAPPEDWINDOW,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        WNDCLASS wc = { 0 };

        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(
            dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
        );

        return (m_hwnd ? TRUE : FALSE);
    }

    HWND Handle() const { return m_hwnd; }

protected:

    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
};

template <class USER_CLASS>
class D2DWindow
{
public:
    D2DWindow() : m_hwnd(NULL) { }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        USER_CLASS* pThis = NULL;

        DC << uMsg << nll;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (USER_CLASS*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = (USER_CLASS*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage_Base(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle = WS_OVERLAPPEDWINDOW,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        WNDCLASS wc = { 0 };

        wc.lpfnWndProc = USER_CLASS::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(
            dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
        );

        return (m_hwnd ? TRUE : FALSE);
    }

    HWND Handle() const { return m_hwnd; }

protected:

    virtual PCWSTR ClassName() const
    {
        return m_ClassName;
    }

    virtual LRESULT HandleMessage_Base(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        static bool destroyed = 0;
        switch (uMsg)
        {
        case WM_CREATE:
            if (FAILED(D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
            {
                return -1;  // Fail CreateWindowEx.
            }
            if (FAILED(DWriteCreateFactory(
                DWRITE_FACTORY_TYPE_SHARED,
                __uuidof(IDWriteFactory),
                reinterpret_cast<IUnknown**>(&pDWriteFactory_))))
            {
                return -1; // Fail CreateWindowEx.
            }
            DPIScale::Initialize(Handle());
            return 0;
        case WM_DESTROY:
            DiscardGraphicsResources_Base();
            SafeRelease(&pFactory);
            PostQuitMessage(0);
            destroyed = 1;
            return 0;
        case WM_PAINT:
            OnPaint_Base();
            return 0;
        case WM_SIZE:
            OnResize();
            return 0;
        }
        if (!destroyed)
        {
            return MKEventSystem(uMsg, wParam, lParam);
        }
    }
    virtual HRESULT CreateGraphicsResources_Base()
    {
        HRESULT hr = S_OK;
        if (!m_pRenderTarget)
        {
            RECT rc;
            GetClientRect(m_hwnd, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            hr = pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(m_hwnd, size),
                &m_pRenderTarget);

            DC.create(m_pRenderTarget, pDWriteFactory_);

            if (SUCCEEDED(hr))
            {
                m_con.create(m_pRenderTarget);      // The default container is created
                hr = CreateGraphicsResources();     // User function
                if (SUCCEEDED(hr))
                {
                    CalculateLayout_Base();
                }
            }
        }
        return hr;
    }
    virtual void DiscardGraphicsResources_Base()
    {
        SafeRelease(&m_pRenderTarget);
        DiscardGraphicsResources();     // User function
    }
    virtual void CalculateLayout_Base()
    {
        if (m_pRenderTarget) CalculateLayout();   // User function
    }
    virtual void OnPaint_Base()
    {
        HRESULT hr = CreateGraphicsResources_Base();
        if (SUCCEEDED(hr))
        {
            PAINTSTRUCT ps;
            BeginPaint(m_hwnd, &ps);

            m_pRenderTarget->BeginDraw();
            m_pRenderTarget->Clear({ 0.1f,0.1f,0.1f,1 });

            DC.display();

            hr = OnPaint();      // User funciton

            hr = m_pRenderTarget->EndDraw();
            if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            {
                DiscardGraphicsResources_Base();
            }
            EndPaint(m_hwnd, &ps);
        }
    }
    virtual void OnResize()
    {
        if (m_pRenderTarget)
        {
            RECT rc;
            GetClientRect(m_hwnd, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            m_pRenderTarget->Resize(size);
            CalculateLayout_Base();
            InvalidateRect(m_hwnd, NULL, FALSE);
        }
    }
    virtual void OnDestroy_Base()
    {
        PostQuitMessage(0);
    }

    // Optional user definitions to extend base functions

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, uMsg, wParam, lParam); };
    virtual HRESULT CreateGraphicsResources() { return S_OK; };
    virtual void DiscardGraphicsResources() {};
    virtual void CalculateLayout() {};
    virtual HRESULT OnPaint() { return S_OK;  };
    virtual void OnDestroy() {};

    HRESULT MKEventSystem(UINT uMsg, WPARAM wParam, LPARAM lParam);     // Mouse and keyboard event system
                                                                        // Automatically distributes mouse and keyboard input to the correct objects

    HWND m_hwnd;
    const wchar_t* m_ClassName = L"Class Name";

    // D2D stuff
    ID2D1Factory* pFactory = NULL;
    IDWriteFactory* pDWriteFactory_ = NULL;
    ID2D1HwndRenderTarget* m_pRenderTarget = NULL;

    // My D2D stuff
    container m_con;

private:
    MKEvent m_lastEvent;            // Do I actually need this
    object* m_lastObject = nullptr; // Used for hover on / off
};

//D3D12Window to come
#include "windoses.cpp"