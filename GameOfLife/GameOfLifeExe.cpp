// GameOfLifeExe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GameOfLifeExe.h"

#include "GameOfLife/GdiRenderer.h"
#include "GameOfLife/InputManager.h"
#include "GameOfLife/Simulation.h"
#include "CellTree/Tree.h"

#include <chrono>
#include <thread>

#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWnd;									    // handle to main window

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GameOfLifeExe, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GameOfLifeExe));

	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE | PM_NOYIELD))
		{
			if (msg.message == WM_QUIT ||
				msg.message == WM_CLOSE ||
				msg.message == WM_DESTROY)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameOfLife::Simulation::GetInstance()->Update();

			wchar_t titleBuffer[128];

			const CellTree::Point& topLeft = GameOfLife::GdiRenderer::GetInstance()->GetTopLeft();
			const CellTree::Tree* tree = GameOfLife::Simulation::GetInstance()->GetTree();
			swprintf_s(titleBuffer, TEXT("Game Of Life - Gen: %lld - Active Cells: %lld - %lld ms - TL: %lld, %lld"), tree->GetGeneration(), tree->GetLastUpdatedCellCount(), tree->GetEvolutionTime(), topLeft.myX, topLeft.myY);
			SetWindowText(hWnd, titleBuffer);

			RedrawWindow(hWnd, 0, 0, RDW_INVALIDATE | RDW_UPDATENOW);

			static int sleepTime = 0;
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		}
	}

	GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GameOfLifeExe));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GameOfLifeExe);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	GameOfLife::Simulation::GetInstance()->Init("input.txt");

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);

				GameOfLife::GdiRenderer::GetInstance()->Render(hdc);

				EndPaint(hWnd, &ps);
			}
			break;
		case WM_SIZE:
			{
				const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
				GameOfLife::GdiRenderer::GetInstance()->SetRenderBounds(pt.x, pt.y);
			}
			break;
		case WM_KEYDOWN:
			{
				GameOfLife::InputManager::GetInstance()->OnKeyDown(wParam);
			}
			break;
		case WM_LBUTTONDOWN:
			{
				const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
				GameOfLife::InputManager::GetInstance()->OnLeftMouseDown(pt.x, pt.y);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				const POINT pt = { LOWORD(lParam), HIWORD(lParam) };
				GameOfLife::InputManager::GetInstance()->OnRightMouseDown(pt.x, pt.y);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				const short delta = GET_WHEEL_DELTA_WPARAM(wParam);
				GameOfLife::InputManager::GetInstance()->OnMouseWheel(delta);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}