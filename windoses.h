// this file makes the creation and manegment of windows easier
#pragma once

#include "LibDosesWin.h"

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
            return 0;
        case WM_PAINT:
            OnPaint_Base();
            return 0;
        case WM_SIZE:
            Resize();
            return 0;
        }
        return HandleMessage(uMsg, wParam, lParam);
    }
    virtual HRESULT CreateGraphicsResources_Base()
    {
        HRESULT hr = S_OK;
        if (pRenderTarget == NULL)
        {
            RECT rc;
            GetClientRect(m_hwnd, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            hr = pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(m_hwnd, size),
                &pRenderTarget);

            if (SUCCEEDED(hr))
            {
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
        SafeRelease(&pRenderTarget);
        DiscardGraphicsResources();     // User function
    }
    virtual void CalculateLayout_Base()
    {
        if (pRenderTarget) CalculateLayout();   // User function
    }
    virtual void OnPaint_Base()
    {
        HRESULT hr = CreateGraphicsResources_Base();
        if (SUCCEEDED(hr))
        {
            PAINTSTRUCT ps;
            BeginPaint(m_hwnd, &ps);

            pRenderTarget->BeginDraw();
            pRenderTarget->Clear({ 0.1f,0.1f,0.1f,1 });

            OnPaint();      // User funciton

            hr = pRenderTarget->EndDraw();
            if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            {
                DiscardGraphicsResources_Base();
            }
            EndPaint(m_hwnd, &ps);
        }
    }
    virtual void Resize()
    {
        if (pRenderTarget)
        {
            RECT rc;
            GetClientRect(m_hwnd, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

            pRenderTarget->Resize(size);
            CalculateLayout_Base();
            InvalidateRect(m_hwnd, NULL, FALSE);
        }
    }

    // Optional user definitions to extend base functions

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, uMsg, wParam, lParam); };
    virtual HRESULT CreateGraphicsResources() { return S_OK; };
    virtual void DiscardGraphicsResources() {};
    virtual void CalculateLayout() {};
    virtual void OnPaint() {};

    HWND m_hwnd;
    const wchar_t* m_ClassName = L"Class Name";
    ID2D1Factory* pFactory = NULL;
    IDWriteFactory* pDWriteFactory_ = NULL;
    ID2D1HwndRenderTarget* pRenderTarget = NULL;
};

//D3D12Window to come
#include "windoses.cpp"