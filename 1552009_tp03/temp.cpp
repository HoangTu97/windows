//// 1552009_tp03.cpp : Defines the entry point for the application.
////
//
//#include "stdafx.h"
//#include "1552009_tp03.h"
//
//#define MAX_LOADSTRING 100
//
//// Global Variables:
//HINSTANCE hInst;                                // current instance
//WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
//WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
//
//												// Forward declarations of functions included in this code module:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//	_In_opt_ HINSTANCE hPrevInstance,
//	_In_ LPWSTR    lpCmdLine,
//	_In_ int       nCmdShow)
//{
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//
//	// TODO: Place code here.
//
//	// Initialize global strings
//	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadStringW(hInstance, IDC_MY1552009_TP03, szWindowClass, MAX_LOADSTRING);
//	MyRegisterClass(hInstance);
//
//	// Perform application initialization:
//	if (!InitInstance(hInstance, nCmdShow))
//	{
//		return FALSE;
//	}
//
//	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1552009_TP03));
//
//	MSG msg;
//
//	// Main message loop:
//	while (GetMessage(&msg, nullptr, 0, 0))
//	{
//		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}
//
//	return (int)msg.wParam;
//}
//
//
//
////
////  FUNCTION: MyRegisterClass()
////
////  PURPOSE: Registers the window class.
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//	WNDCLASSEXW wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	wcex.style = CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc = WndProc;
//	wcex.cbClsExtra = 0;
//	wcex.cbWndExtra = 0;
//	wcex.hInstance = hInstance;
//	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1552009_TP03));
//	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY1552009_TP03);
//	wcex.lpszClassName = szWindowClass;
//	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//	return RegisterClassExW(&wcex);
//}
//
////
////   FUNCTION: InitInstance(HINSTANCE, int)
////
////   PURPOSE: Saves instance handle and creates main window
////
////   COMMENTS:
////
////        In this function, we save the instance handle in a global variable and
////        create and display the main program window.
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//	hInst = hInstance; // Store instance handle in our global variable
//
//	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
//
//	if (!hWnd)
//	{
//		return FALSE;
//	}
//
//	ShowWindow(hWnd, nCmdShow);
//	UpdateWindow(hWnd);
//
//	return TRUE;
//}
//
//
//struct Shape {
//	COLORREF color;
//	COLORREF fill;
//	bool isFill;
//	bool isHidden;
//	int method;
//	DWORD penStyle;
//	DWORD width;
//	std::vector<POINT> arrPoint;
//};
//std::vector<Shape> Shapes;
//int tempMethod = DRAW_LINE;
//COLORREF tempColor = RGB(0, 0, 0);
//COLORREF tempFill = RGB(0, 0, 0);
//bool tempIsFill = false;
//bool down = false;
//int statusPos = 0;
//bool isMove = false;
//POINT LastMove;
//DWORD tempPenStyle = PS_SOLID;
//DWORD tempWidth = 5;
//int indexShape = 0;
////
////  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  PURPOSE:  Processes messages for the main window.
////
////  WM_COMMAND  - process the application menu
////  WM_PAINT    - Paint the main window
////  WM_DESTROY  - post a quit message and return
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	PAINTSTRUCT ps;
//	HDC hdc = GetDC(hWnd);
//	bool isHidden = false;
//	LOGBRUSH lb;
//	RECT rcClient;
//	GetClientRect(hWnd, &rcClient);
//	HDC bufDC;
//	HBITMAP bufBMP;
//	bufDC = CreateCompatibleDC(hdc);
//	bufBMP = CreateCompatibleBitmap(hdc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
//
//	switch (message)
//	{
//	case WM_COMMAND:
//	{
//		int wmId = LOWORD(wParam);
//		// Parse the menu selections:
//		switch (wmId)
//		{
//		case IDM_ABOUT:
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case ID_SHAPE_LINE:
//			tempIsFill = false;
//			tempMethod = DRAW_LINE;
//			if (!Shapes.empty())
//				if (Shapes.back().arrPoint.size() % 2 != 0)
//					Shapes.pop_back();
//			break;
//		case ID_SHAPE_RECTANGLE:
//			tempMethod = DRAW_RECT;
//			if (!Shapes.empty())
//				if (Shapes.back().arrPoint.size() % 2 != 0)
//					Shapes.pop_back();
//			break;
//		case ID_SHAPE_ELLIPSE:
//			tempMethod = DRAW_ELLIPSE;
//			if (!Shapes.empty())
//				if (Shapes.back().arrPoint.size() % 2 != 0)
//					Shapes.pop_back();
//			break;
//		case ID_SHAPE_RANDOM:
//			tempIsFill = false;
//			tempMethod = DRAW_RANDOM;
//			break;
//		case ID_COLOR_BLACK:
//			tempColor = RGB(0, 0, 0);
//			break;
//		case ID_COLOR_RED:
//			tempColor = RGB(255, 0, 0);
//			break;
//		case ID_COLOR_GREEN:
//			tempColor = RGB(0, 255, 0);
//			break;
//		case ID_COLOR_BLUE:
//			tempColor = RGB(0, 0, 255);
//			break;
//		case ID_FILL_BLACK:
//			tempIsFill = true;
//			tempFill = RGB(0, 0, 0);
//			break;
//		case ID_FILL_RED:
//			tempIsFill = true;
//			tempFill = RGB(255, 0, 0);
//			break;
//		case ID_FILL_GREEN:
//			tempIsFill = true;
//			tempFill = RGB(0, 255, 0);
//			break;
//		case ID_FILL_BLUE:
//			tempIsFill = true;
//			tempFill = RGB(0, 0, 255);
//			break;
//		case ID_FILL_NONE:
//			tempIsFill = false;
//			break;
//		case ID_EDIT_UNDO:
//			if (statusPos > 0) {
//				Shapes[statusPos - 1].isHidden = true;
//				statusPos--;
//				RECT rc;
//				GetClientRect(hWnd, &rc);
//				InvalidateRect(hWnd, &rc, TRUE);
//				UpdateWindow(hWnd);
//			}
//			break;
//		case ID_EDIT_REDO:
//			if (statusPos < Shapes.size()) {
//				Shapes[statusPos].isHidden = false;
//				statusPos++;
//				RECT rc;
//				GetClientRect(hWnd, &rc);
//				InvalidateRect(hWnd, &rc, TRUE);
//				UpdateWindow(hWnd);
//			}
//			break;
//		case ID_EDIT_DRAW:
//			isMove = false;
//			break;
//		case ID_MOVE_MOVE:
//			isMove = true;
//			break;
//		case ID_MOVE_UP:
//			isMove = true;
//			indexShape = (indexShape + 1) % Shapes.size();
//			break;
//		case ID_MOVE_DOWN:
//			isMove = true;
//			if (indexShape > 0) {
//				indexShape--;
//			}
//			else {
//				indexShape = Shapes.size() - 1;
//			}
//			break;
//		case ID_STYLE_SOLID:
//			tempPenStyle = PS_SOLID;
//			break;
//		case ID_STYLE_DASH:
//			tempPenStyle = PS_DASH;
//			break;
//		case ID_STYLE_DOT:
//			tempPenStyle = PS_DOT;
//			break;
//		case ID_STYLE_DASHDOT:
//			tempPenStyle = PS_DASHDOT;
//			break;
//		case ID_STYLE_DASHDOTDOT:
//			tempPenStyle = PS_DASHDOTDOT;
//			break;
//		case ID_STYLE_INSIDEFRAME:
//			tempPenStyle = PS_INSIDEFRAME;
//			break;
//		case ID_STYLE_NONE:
//			tempPenStyle = PS_NULL;
//			break;
//		case ID_WIDTH_1:
//			tempWidth = 1;
//			break;
//		case ID_WIDTH_2:
//			tempWidth = 2;
//			break;
//		case ID_WIDTH_3:
//			tempWidth = 3;
//			break;
//		case ID_WIDTH_4:
//			tempWidth = 4;
//			break;
//		case ID_WIDTH_5:
//			tempWidth = 5;
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//	}
//	break;
//	case WM_LBUTTONDOWN:
//	{
//		POINT a;
//		a.x = LOWORD(lParam);
//		a.y = HIWORD(lParam);
//		if (!isMove) {
//			while (statusPos != Shapes.size())
//				Shapes.pop_back();
//			Shape New;
//			Shapes.push_back(New);
//			Shapes.back().arrPoint.push_back(a);
//			if (Shapes.back().arrPoint.size() % 2 == 1) {
//				Shapes.back().color = tempColor;
//				Shapes.back().method = tempMethod;
//				Shapes.back().fill = tempFill;
//				Shapes.back().isFill = tempIsFill;
//				Shapes.back().isHidden = false;
//				Shapes.back().penStyle = tempPenStyle;
//				Shapes.back().width = tempWidth;
//				indexShape = Shapes.size() - 1;
//				statusPos++;
//			}
//		}
//		else {
//			LastMove = a;
//		}
//		down = true;
//		break;
//	}
//	case WM_LBUTTONUP:
//	{
//		POINT a;
//		a.x = LOWORD(lParam);
//		a.y = HIWORD(lParam);
//		if (!isMove && Shapes.back().method != DRAW_RANDOM) {
//			if (Shapes.back().arrPoint.size() % 2 == 0
//				&& Shapes.back().arrPoint.size() % 2 == 0) {
//				Shapes.back().arrPoint.back() = a;
//			}
//			else if (Shapes.back().arrPoint.size() % 2 != 0) {
//				Shapes.back().arrPoint.push_back(a);
//			}
//		}
//		down = false;
//		break;
//	}
//	case WM_MOUSEMOVE:
//	{
//		if (Shapes.empty()) break;
//
//		POINT a;
//		a.x = LOWORD(lParam);
//		a.y = HIWORD(lParam);
//
//		if (!isMove) {
//			if (Shapes.back().method != DRAW_RANDOM) {
//				if (Shapes.back().arrPoint.size() % 2 != 0) {
//					Shapes.back().arrPoint.push_back(a);
//				}
//				else if (Shapes.back().arrPoint.size() % 2 == 0
//					&& Shapes.back().arrPoint.size() > 0) {
//					if (down) {
//						Shapes.back().arrPoint.back() = a;
//					}
//				}
//			}
//			else {
//				if (down) {
//					Shapes.back().arrPoint.push_back(a);
//				}
//			}
//		}
//		else if (isMove && down) {
//			long diffX = a.x - LastMove.x;
//			long diffY = a.y - LastMove.y;
//			for (int i = 0; i < Shapes[indexShape].arrPoint.size(); i++) {
//				Shapes[indexShape].arrPoint[i].x += diffX;
//				Shapes[indexShape].arrPoint[i].y += diffY;
//			}
//			LastMove = a;
//		}
//	}
//	case WM_PAINT:
//	{
//		SelectObject(bufDC, bufBMP);
//		FillRect(bufDC, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));
//		for (int i = 0; i < Shapes.size(); i++) {
//			/*HRGN hRegion;
//			HBRUSH hbrush;*/
//			if (!Shapes[i].isHidden) {
//				lb.lbStyle = BS_SOLID;
//				lb.lbColor = Shapes[i].color;
//				lb.lbHatch = 0;
//				HPEN hPen = ExtCreatePen(PS_GEOMETRIC | Shapes[i].penStyle, Shapes[i].width, &lb, 0, NULL);
//				SelectObject(bufDC, hPen);
//				SelectObject(bufDC, GetStockObject(NULL_BRUSH));
//				switch (Shapes[i].method)
//				{
//				case DRAW_LINE:
//					MoveToEx(bufDC, (int)Shapes[i].arrPoint[0].x, (int)Shapes[i].arrPoint[0].y, NULL);
//					LineTo(bufDC, (int)Shapes[i].arrPoint[1].x, (int)Shapes[i].arrPoint[1].y);
//					break;
//				case DRAW_RECT:
//					if (Shapes[i].isFill) {
//						SelectObject(bufDC, GetStockObject(DC_BRUSH));
//						SetDCBrushColor(bufDC, Shapes[i].fill);
//					}
//					Rectangle(bufDC,
//						(int)Shapes[i].arrPoint[0].x, (int)Shapes[i].arrPoint[0].y,
//						(int)Shapes[i].arrPoint[1].x, (int)Shapes[i].arrPoint[1].y);
//					break;
//				case DRAW_ELLIPSE:
//					if (Shapes[i].isFill) {
//						SelectObject(bufDC, GetStockObject(DC_BRUSH));
//						SetDCBrushColor(bufDC, Shapes[i].fill);
//					}
//					Ellipse(bufDC,
//						(int)Shapes[i].arrPoint[0].x, (int)Shapes[i].arrPoint[0].y,
//						(int)Shapes[i].arrPoint[1].x, (int)Shapes[i].arrPoint[1].y);
//					break;
//				case DRAW_RANDOM:
//					for (int j = 0; j < Shapes[i].arrPoint.size() - 1; j++) {
//						MoveToEx(bufDC, (int)Shapes[i].arrPoint[j].x, (int)Shapes[i].arrPoint[j].y, NULL);
//						LineTo(bufDC, (int)Shapes[i].arrPoint[j + 1].x, (int)Shapes[i].arrPoint[j + 1].y);
//					}
//					break;
//				}
//				DeleteObject(hPen);
//			}
//		}
//
//		BitBlt(hdc, 0, 0,
//			rcClient.right - rcClient.left,
//			rcClient.bottom - rcClient.top,
//			bufDC, 0, 0, SRCCOPY);
//
//		hdc = BeginPaint(hWnd, &ps);
//		// TODO: Add any drawing code that uses hdc here...
//		EndPaint(hWnd, &ps);
//	}
//	break;
//	case WM_DESTROY:
//		DeleteObject(bufBMP);
//		DeleteDC(bufDC);
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}
//
//// Message handler for about box.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}