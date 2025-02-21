// Popcorn.cpp : Defines the entry point for the application.
//

#include "Main.h"
#include "../Engine/Engine.h"

#include <Windows.h>
#include <iostream>
#include <minwindef.h>
#include <windef.h>
#include <windows.h>
#include <wingdi.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst; // current instance
AsEngine Engine;
WCHAR szTitle[MAX_LOADSTRING];       // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPWSTR lpCmdLine, int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.
  AsConfig::Setup_Colors();

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_POPCORN, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable =
      LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POPCORN));

  MSG msg;

  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

// Регистрация класса окна
ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POPCORN));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  // wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
  wcex.hbrBackground = AsConfig::BG_Brush;
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_POPCORN);
  wcex.lpszClassName = L"MainWndClass";
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

// Инициализация экземпляра
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {

  RECT window_rect;
  window_rect.left = 0;
  window_rect.top = 0;
  window_rect.right = 320 * 3;
  window_rect.bottom = 200 * 3;
  std::cout << "window_rect.right = " << window_rect.right << std::endl;

  AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

  HWND hWnd = CreateWindowW(L"MainWndClass", L"Popcorn", WS_OVERLAPPEDWINDOW, 0,
                            0, window_rect.right - window_rect.left,
                            window_rect.bottom - window_rect.top, nullptr,
                            nullptr, hInstance, nullptr);

  if (hWnd == 0) {
    return FALSE;
  }

  Engine.Init_Engine(hWnd);

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

//------------------------------------------------------------------------------------------------------------
//

// Обработчик сообщений окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {

  int wmId;
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message) {
  case WM_COMMAND: {
    wmId = LOWORD(wParam);
    // Разобрать выбор в меню:
    switch (wmId) {
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  } break;
  case WM_PAINT: {

    hdc = BeginPaint(hWnd, &ps);

    // TODO: Добавьте сюда любой код прорисовки, использующий hdc...

    Engine.Draw_Frame(hdc, ps.rcPaint);

    EndPaint(hWnd, &ps);
  } break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_KEYDOWN:

    switch (wParam) {
    case VK_LEFT:
      return Engine.On_Key_Down(EKT_Left);
    case VK_RIGHT:
      return Engine.On_Key_Down(EKT_Right);
    case VK_SPACE:
      return Engine.On_Key_Down(EKT_Space);
    }
    break;

  case WM_TIMER:
    if (wParam == Timer_ID) {
      return Engine.On_Timer();
    }
    break;

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}
//------------------------------------------------------------------------------------------------------------
