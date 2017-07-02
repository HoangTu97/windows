// 1552009_tp03.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1552009_tp03.h"
#include "Shape.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//main avariable
std::vector<Shape *> Shapes;
int tempMethod = DRAW_RANDOM;
COLORREF tempColor = RGB(0, 0, 0);
COLORREF tempFill = RGB(0, 0, 0);
bool tempIsFill = false;
bool down = false;
int statusPos = 0;
int control = CONTROL_DRAW;
POINT LastMove;
DWORD tempPenStyle = PS_SOLID;
DWORD tempWidth = 5;
LOGBRUSH lb;
HDC bufDC;
HBITMAP bufBMP;
LPWSTR filename;
TCHAR szFile[260];
std::vector<int> selectShape;
RECT selectZone;
//Format pen menu
DWORD formatPenWidth;
COLORREF formatPenColor;
DWORD formatPenStyle;
//Format brush menu
LOGBRUSH formatBrush;
bool formatBrushIsFill;














// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Pen(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Brush(HWND, UINT, WPARAM, LPARAM);
VOID ColorPickerDialog(HWND, COLORREF &);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1552009_TP03, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1552009_TP03));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1552009_TP03));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1552009_TP03);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

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
	PAINTSTRUCT ps;
	HDC hdc = GetDC(hWnd);
	bool isHidden = false;
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	

    switch (message)
    {
	case WM_CREATE:
	{
		lb.lbColor = tempFill;
		lb.lbHatch = NULL;
		lb.lbStyle = BS_SOLID;
		bufDC = CreateCompatibleDC(hdc);
		bufBMP = CreateCompatibleBitmap(hdc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
		return 0;
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case ID_FORMAT_PEN:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_PEN), hWnd, Pen);
				break;
			case ID_FORMAT_BRUSH:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_BRUSH), hWnd, Brush);
				break;
			case ID_FILE_SAVE:
			{
				control = CONTROL_DRAW;
				OPENFILENAME ofn;
				TCHAR szFile[260];
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = _T("All Files *.*\0*.*\0Text Files *.txt\0*.TXT\0 Doc Files\0*.TXT;*.DOC;*.BAK\0");
				ofn.nFilterIndex = 1;
				ofn.lpstrInitialDir = _T("C:\\");
				ofn.lpstrTitle = _T("My Application - Save file");
				ofn.lpstrDefExt = _T("txt");
				// Display the Open dialog box. 
				if (GetSaveFileName(&ofn) == TRUE)
				{
					std::ofstream file;
					file.open(ofn.lpstrFile, std::ios::out | std::ios::binary);

					for (int i = 0; i < Shapes.size(); i++) {
						file.write((char*)&Shapes[i]->type, sizeof(Shapes[i]->type));
						file.write(",", 1);
						file.write((char*)&Shapes[i]->hidden, sizeof(Shapes[i]->hidden));
						file.write(",", 1);
						file.write((char*)&Shapes[i]->penStyle, sizeof(Shapes[i]->penStyle));
						file.write(",", 1);
						file.write((char*)&Shapes[i]->width, sizeof(Shapes[i]->width));
						file.write(",", 1);
						file.write((char*)&Shapes[i]->color, sizeof(Shapes[i]->color));
						file.write(",", 1);
						char check = Shapes[i]->checkFill();
						file.write((char*)&check, sizeof(check));
						file.write(",", 1);
						if (Shapes[i]->checkFill()) {
							lb = Shapes[i]->getBrush();
							file.write((char*)&lb.lbStyle, sizeof(lb.lbStyle));
							file.write(",", 1);
							file.write((char*)&lb.lbHatch, sizeof(lb.lbHatch));
							file.write(",", 1);
							file.write((char*)&lb.lbColor, sizeof(lb.lbColor));
							file.write(",", 1);
						}

						int size = Shapes[i]->arrPoint.size();
						file.write((char*)&size, sizeof(size));
						file.write(",", 1);
						for (int j = 0; j < size; j++) {
							file.write((char*)&Shapes[i]->arrPoint[j].x, sizeof(Shapes[i]->arrPoint[j].x));
							file.write(",", 1);
							file.write((char*)&Shapes[i]->arrPoint[j].y, sizeof(Shapes[i]->arrPoint[j].y));
							file.write(",", 1);
						}
						file.write("\n", 1);
					}
					file.close();
				}
				break;
			}
			case ID_FILE_SAVEBMP:
			{
				control = CONTROL_DRAW;
				OPENFILENAME ofn;
				TCHAR szFile[260];
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = _T("All Files *.*\0*.*\0Text Files *.txt\0*.TXT\0 Doc Files\0*.TXT;*.DOC;*.BAK\0");
				ofn.nFilterIndex = 1;
				ofn.lpstrInitialDir = _T("C:\\");
				ofn.lpstrTitle = _T("My Application - Save file");
				ofn.lpstrDefExt = _T("txt");
				// Display the Open dialog box. 
				if (GetSaveFileName(&ofn) == TRUE)
				{
					// get screen rectangle
					RECT windowRect;
					GetClientRect(hWnd, &windowRect);

					// bitmap dimensions
					int bitmap_dx = windowRect.right - windowRect.left;
					int bitmap_dy = windowRect.bottom - windowRect.top;

					// create file
					std::ofstream file(ofn.lpstrFile, std::ios::out | std::ios::binary);
					if (!file) return 0;

					// save bitmap file headers
					BITMAPFILEHEADER fileHeader;
					BITMAPINFOHEADER infoHeader;

					fileHeader.bfType = 0x4d42;
					fileHeader.bfSize = 0;
					fileHeader.bfReserved1 = 0;
					fileHeader.bfReserved2 = 0;
					fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

					infoHeader.biSize = sizeof(infoHeader);
					infoHeader.biWidth = bitmap_dx;
					infoHeader.biHeight = bitmap_dy;
					infoHeader.biPlanes = 1;
					infoHeader.biBitCount = 24;
					infoHeader.biCompression = BI_RGB;
					infoHeader.biSizeImage = 0;
					infoHeader.biXPelsPerMeter = 0;
					infoHeader.biYPelsPerMeter = 0;
					infoHeader.biClrUsed = 0;
					infoHeader.biClrImportant = 0;

					file.write((char*)&fileHeader, sizeof(fileHeader));
					file.write((char*)&infoHeader, sizeof(infoHeader));

					// dibsection information
					BITMAPINFO info;
					info.bmiHeader = infoHeader;

					// ------------------
					// THE IMPORTANT CODE
					// ------------------
					// create a dibsection and blit the window contents to the bitmap
					HDC winDC = GetDC(hWnd);
					HDC memDC = CreateCompatibleDC(winDC);
					BYTE* memory = 0;
					HBITMAP bitmap = CreateDIBSection(winDC, &info, DIB_RGB_COLORS, (void**)&memory, 0, 0);
					SelectObject(memDC, bitmap);
					BitBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC, 0, 0, SRCCOPY);
					DeleteDC(memDC);
					ReleaseDC(hWnd, winDC);

					// save dibsection data
					int bytes = (((24 * bitmap_dx + 31) & (~31)) / 8)*bitmap_dy;
					file.write((char*)memory, bytes);

					// HA HA, forgot paste in the DeleteObject lol, happy now ;)?
					DeleteObject(bitmap);
				}
				break;
			}
			case ID_FILE_OPEN:
			{
				control = CONTROL_DRAW;
				OPENFILENAME ofn;
				TCHAR szFile[260];
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = _T("All Files *.*\0*.*\0Text Files *.txt\0*.TXT\0 Doc Files\0*.TXT;*.DAT;*.BMP\0");
				ofn.nFilterIndex = 1;
				ofn.lpstrInitialDir = _T("C:\\");
				ofn.lpstrTitle = _T("My Application - Open file");
				ofn.lpstrDefExt = _T("txt");
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				// Display the Open dialog box. 
				if (GetOpenFileName(&ofn) == TRUE)
				{
					std::ifstream file;
					file.open(ofn.lpstrFile);
					if (file) {

						while (int i = Shapes.size()) {
							delete Shapes[i - 1];
							Shapes.pop_back();
						}

						statusPos = 0;

						while (!file.eof()) {
							int method = 0;
							char comma;
							file.read((char*)&method, sizeof(method));
							file.read((char*)&comma, 1);
							if (!method)
								break;
							Shape * New = Shape::factory(method);

							Shapes.push_back(New);
							Shapes.back()->type = method;
							file.read((char*)&Shapes.back()->hidden, sizeof(Shapes.back()->hidden));
							file.read((char*)&comma, 1);
							file.read((char*)&Shapes.back()->penStyle, sizeof(Shapes.back()->penStyle));
							file.read((char*)&comma, 1);
							file.read((char*)&Shapes.back()->width, sizeof(Shapes.back()->width));
							file.read((char*)&comma, 1);
							file.read((char*)&Shapes.back()->color, sizeof(Shapes.back()->color));
							file.read((char*)&comma, 1);
							char isFill;
							file.read((char*)&isFill, sizeof(isFill));
							file.read((char*)&comma, 1);
							if (isFill) {
								file.read((char*)&lb.lbStyle, sizeof(lb.lbStyle));
								file.read((char*)&comma, 1);
								file.read((char*)&lb.lbHatch, sizeof(lb.lbHatch));
								file.read((char*)&comma, 1);
								file.read((char*)&lb.lbColor, sizeof(lb.lbColor));
								file.read((char*)&comma, 1);
								Shapes.back()->setBrush(lb);
							}
							int size;
							file.read((char*)&size, sizeof(size));
							file.read((char*)&comma, 1);
							for (int j = 0; j < size; j++) {
								POINT pt;
								file.read((char*)&pt.x, sizeof(pt.x));
								file.read((char*)&comma, 1);
								file.read((char*)&pt.y, sizeof(pt.y));
								file.read((char*)&comma, 1);
								Shapes.back()->arrPoint.push_back(pt);
							}
							file.read((char*)&comma, 1);
							statusPos++;
						}
						file.close();
					}

					redraw(hWnd, hdc, Shapes);

					MessageBox(NULL, ofn.lpstrFile, _T("Selected file"), MB_OK);
				}
				break;
			}
			case ID_FILE_OPENBMP:
			{
				control = CONTROL_DRAW;
				OPENFILENAME ofn;
				TCHAR szFile[260];
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = _T("All Files *.*\0*.*\0Text Files *.txt\0*.TXT\0 Doc Files\0*.TXT;*.DAT;*.BMP\0");
				ofn.nFilterIndex = 1;
				ofn.lpstrInitialDir = _T("C:\\");
				ofn.lpstrTitle = _T("My Application - Open file");
				ofn.lpstrDefExt = _T("txt");
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				// Display the Open dialog box. 
				if (GetOpenFileName(&ofn) == TRUE)
				{
					filename = ofn.lpstrFile;

					while (int i = Shapes.size()) {
						delete Shapes[i - 1];
						Shapes.pop_back();
					}

					statusPos = 0;

					Shape * New = Shape::factory(DRAW_IMAGE);

					Shapes.push_back(New);
					Shapes.back()->type = DRAW_IMAGE;
					Shapes.back()->hidden = false;
					Shapes.back()->setFilename(filename);
					POINT pt = { 0,0 };
					Shapes.back()->arrPoint.push_back(pt);
					Shapes.back()->arrPoint.push_back(pt);
					statusPos++;

					redraw(hWnd, hdc, Shapes);

					MessageBox(NULL, ofn.lpstrFile, _T("Selected file"), MB_OK);
				}
				break;
			}
			case ID_SHAPE_LINE:
			{
				control = CONTROL_DRAW;
				tempIsFill = false;
				tempMethod = DRAW_LINE;
				if (!Shapes.empty())
					if (Shapes.back()->numPoint() < 2)
						Shapes.pop_back();
				break;
			}
			case ID_SHAPE_RECTANGLE:
			{
				control = CONTROL_DRAW;
				tempMethod = DRAW_RECT;
				if (!Shapes.empty())
					if (Shapes.back()->numPoint() < 2)
						Shapes.pop_back();
				break;
			}
			case ID_SHAPE_ELLIPSE:
			{
				control = CONTROL_DRAW;
				tempMethod = DRAW_ELLIPSE;
				if (!Shapes.empty())
					if (Shapes.back()->numPoint() < 2)
						Shapes.pop_back();
				break;
			}
			case ID_SHAPE_RANDOM:
			{
				control = CONTROL_DRAW;
				tempIsFill = false;
				tempMethod = DRAW_RANDOM;
				if (!Shapes.empty())
					if (Shapes.back()->numPoint() < 2)
						Shapes.pop_back();
				break;
			}
			case ID_SHAPE_SQUARE:
			{
				control = CONTROL_DRAW;
				tempIsFill = false;
				tempMethod = DRAW_SQUARE;
				if (!Shapes.empty())
					if (Shapes.back()->numPoint() < 2)
						Shapes.pop_back();
				break;
			}
			case ID_SHAPE_ROUND:
			{
				control = CONTROL_DRAW;
				tempIsFill = false;
				tempMethod = DRAW_ROUND;
				if (!Shapes.empty())
					if (Shapes.back()->numPoint() < 2)
						Shapes.pop_back();
				break;
			}
			case ID_COLOR_BLACK:
				control = CONTROL_DRAW;
				tempColor = RGB( 0,0,0 );
				break;
			case ID_COLOR_RED:
				control = CONTROL_DRAW;
				tempColor = RGB( 255,0,0 );
				break;
			case ID_COLOR_GREEN:
				control = CONTROL_DRAW;
				tempColor = RGB( 0,255,0 );
				break;
			case ID_COLOR_BLUE:
				control = CONTROL_DRAW;
				tempColor = RGB( 0,0,255 );
				break;
			case ID_FILL_BLACK:
				control = CONTROL_DRAW;
				tempIsFill = true;
				lb.lbColor = RGB( 0,0,0 );
				break;
			case ID_FILL_RED:
				control = CONTROL_DRAW;
				tempIsFill = true;
				lb.lbColor = RGB( 255,0,0 );
				break;
			case ID_FILL_GREEN:
				control = CONTROL_DRAW;
				tempIsFill = true;
				lb.lbColor = RGB( 0,255,0 );
				break;
			case ID_FILL_BLUE:
				control = CONTROL_DRAW;
				tempIsFill = true;
				lb.lbColor = RGB( 0,0,255 );
				break;
			case ID_FILL_NONE:
				control = CONTROL_DRAW;
				tempIsFill = false;
				break;
			case ID_EDIT_UNDO:
				control = CONTROL_DRAW;
				if (statusPos > 0) {
					Shapes[statusPos - 1]->setHidden();
					statusPos--;

					redraw(hWnd, hdc, Shapes);
				}
				break;
			case ID_EDIT_REDO:
				control = CONTROL_DRAW;
				if (statusPos < Shapes.size()) {
					Shapes[statusPos]->setVisible();
					statusPos++;

					redraw(hWnd, hdc, Shapes);
				}
				break;
			case ID_EDIT_DRAW:
				control = CONTROL_DRAW;
				break;
			case ID_EDIT_MOVE:
				control = CONTROL_MOVE;
				break;
			case ID_EDIT_SCALE:
				control = CONTROL_SCALE;
				break;
			case ID_EDIT_SELECT:
			{
				control = CONTROL_SELECT;
				while (selectShape.size())
					selectShape.pop_back();
				break;
			}
			case ID_STYLE_SOLID:
				control = CONTROL_DRAW;
				tempPenStyle = PS_SOLID;
				break;
			case ID_STYLE_DASH:
				control = CONTROL_DRAW;
				tempPenStyle = PS_DASH;
				break;
			case ID_STYLE_DOT:
				control = CONTROL_DRAW;
				tempPenStyle = PS_DOT;
				break;
			case ID_STYLE_DASHDOT:
				control = CONTROL_DRAW;
				tempPenStyle = PS_DASHDOT;
				break;
			case ID_STYLE_DASHDOTDOT:
				control = CONTROL_DRAW;
				tempPenStyle = PS_DASHDOTDOT;
				break;
			case ID_STYLE_INSIDEFRAME:
				control = CONTROL_DRAW;
				tempPenStyle = PS_INSIDEFRAME;
				break;
			case ID_STYLE_NONE:
				control = CONTROL_DRAW;
				tempPenStyle = PS_NULL;
				break;
			case ID_WIDTH_1:
				control = CONTROL_DRAW;
				tempWidth = 1;
				break;
			case ID_WIDTH_2:
				control = CONTROL_DRAW;
				tempWidth = 2;
				break;
			case ID_WIDTH_3:
				control = CONTROL_DRAW;
				tempWidth = 3;
				break;
			case ID_WIDTH_4:
				control = CONTROL_DRAW;
				tempWidth = 4;
				break;
			case ID_WIDTH_5:
				control = CONTROL_DRAW;
				tempWidth = 5;
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_LBUTTONDOWN:
	{
		POINT a;
		a.x = LOWORD(lParam);
		a.y = HIWORD(lParam);
		switch (control) {
		case CONTROL_DRAW:
		{
			while (statusPos != Shapes.size())
				Shapes.pop_back();

			Shape * New = Shape::factory(tempMethod);

			Shapes.push_back(New);
			Shapes.back()->addPoint(a);
			if (Shapes.back()->numPoint() % 2 == 1) {
				Shapes.back()->type = tempMethod;
				Shapes.back()->setColor(tempColor);
				Shapes.back()->setIsFill(tempIsFill);
				Shapes.back()->setBrush(lb);
				Shapes.back()->setVisible();
				Shapes.back()->setStyle(tempPenStyle);
				Shapes.back()->setWidth(tempWidth);
				statusPos++;
			}
			break;
		}
		case CONTROL_SELECT:
		{
			selectZone.left = a.x;
			selectZone.top = a.y;
			break;
		}
		default:
			LastMove = a;
			break;
		}
		

		down = true;
		break;
	}
	case WM_LBUTTONUP:
	{
		POINT a;
		a.x = LOWORD(lParam);
		a.y = HIWORD(lParam);
		switch (control) {
		case CONTROL_DRAW:
		{
			Shapes.back()->MouseUp(a);
			break;
		}
		case CONTROL_SELECT:
		{
			selectZone.right = a.x;
			selectZone.bottom = a.y;

			if (selectZone.left > selectZone.right) {
				long temp = selectZone.left;
				selectZone.left = selectZone.right;
				selectZone.right = temp;
			}
			if (selectZone.top > selectZone.bottom) {
				long temp = selectZone.top;
				selectZone.top = selectZone.bottom;
				selectZone.bottom = temp;
			}

			for (int i = 0; i < Shapes.size(); i++) {
				if (Shapes[i]->isSelected(selectZone)) {
					selectShape.push_back(i);
				}
			}
		}
		}
		down = false;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (!down) break;
		if (Shapes.empty()) break;

		POINT a;
		a.x = LOWORD(lParam);
		a.y = HIWORD(lParam);
		
		switch (control) {
		case CONTROL_DRAW:
			Shapes.back()->MouseMove(a);
			break;
		case CONTROL_MOVE:
		{
			long diffX = a.x - LastMove.x;
			long diffY = a.y - LastMove.y;

			for (int i = 0; i < selectShape.size(); i++) {
				Shapes[selectShape[i]]->move(diffX, diffY);
			}

			LastMove = a;
			break;
		}
		case CONTROL_SCALE:
		{
			long diffX = a.x - LastMove.x;
			long diffY = a.y - LastMove.y;

			for (int i = 0; i < selectShape.size(); i++) {
				Shapes[selectShape[i]]->scale(diffX, diffY);
			}

			LastMove = a;
			break;
		}
		case CONTROL_SELECT:
		{
			selectZone.right = a.x;
			selectZone.bottom = a.y;
			break;
		}
		default:
			return 0;
		}
	}
    case WM_PAINT:
        {				
			// clear screen
			SelectObject(bufDC, bufBMP);
			FillRect(bufDC, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));


			// main draw
			for (int i = 0; i < Shapes.size(); i++) {
				Shapes[i]->draw(bufDC);
			}
			

			// copy buffer
			BitBlt(hdc, 0, 0, 
				rcClient.right - rcClient.left, 
				rcClient.bottom - rcClient.top, 
				bufDC, 0, 0, SRCCOPY);

            hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
			
        }
        break;
    case WM_DESTROY:

		DeleteObject(bufBMP);
		DeleteDC(bufDC);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}




// Message handler for pen box.
INT_PTR CALLBACK Pen(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		formatPenWidth = tempWidth;
		formatPenColor = tempColor;
		formatPenStyle = tempPenStyle;
		return (INT_PTR)TRUE;
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (pDIS->CtlID == IDC_PREVIEW_PEN)
		{
			RECT rcClient;
			GetClientRect(hDlg, &rcClient);
			FillRect(pDIS->hDC, &rcClient, (HBRUSH)(COLOR_WINDOW));
			lb.lbStyle = BS_SOLID;
			lb.lbColor = formatPenColor;
			lb.lbHatch = 0;
			HPEN hPen = ExtCreatePen(PS_GEOMETRIC | formatPenStyle, formatPenWidth, &lb, 0, NULL);
			SelectObject(pDIS->hDC, hPen);
			MoveToEx(pDIS->hDC, 10, 30, NULL);
			LineTo(pDIS->hDC, 150, 30);
		}
		else if (pDIS->CtlID == IDC_PREVIEW_PEN_COLOR) {
			RECT rcClient;
			GetClientRect(hDlg, &rcClient);
			FillRect(pDIS->hDC, &rcClient, (HBRUSH)(COLOR_WINDOW));
			SelectObject(pDIS->hDC, GetStockObject(DC_BRUSH));
			SetDCBrushColor(pDIS->hDC, formatPenColor);
			Rectangle(pDIS->hDC,
				0, 0,
				70, 30);
		}
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_PEN_WIDTH:
		{
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
				TCHAR szBuffer[256];
				formatPenWidth = GetDlgItemInt(hDlg, IDC_PEN_WIDTH, NULL, NULL);
				RECT rect;
				HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_PEN);
				GetClientRect(hControl, &rect);
				InvalidateRect(hControl, &rect, TRUE);
				UpdateWindow(hControl);
				break;
			}
			break;
		}
		case IDC_COLORPICKER:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				ColorPickerDialog(hDlg, formatPenColor);
				RECT rect;
				HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_PEN);
				GetClientRect(hControl, &rect);
				InvalidateRect(hControl, &rect, TRUE);
				UpdateWindow(hControl);
			}
			}
			break;
		}
		case IDC_PEN_STYLE_SOLID:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				formatPenStyle = PS_SOLID;
				RECT rect;
				HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_PEN);
				GetClientRect(hControl, &rect);
				InvalidateRect(hControl, &rect, TRUE);
				UpdateWindow(hControl);
			}
			}
			break;
		}
		case IDC_PEN_STYLE_DASH:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				formatPenStyle = PS_DASH;
				RECT rect;
				HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_PEN);
				GetClientRect(hControl, &rect);
				InvalidateRect(hControl, &rect, TRUE);
				UpdateWindow(hControl);
			}
			}
			break;
		}
		case IDC_PEN_STYLE_DOT:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				formatPenStyle = PS_DOT;
				RECT rect;
				HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_PEN);
				GetClientRect(hControl, &rect);
				InvalidateRect(hControl, &rect, TRUE);
				UpdateWindow(hControl);
			}
			}
			break;
		}
		case IDC_PEN_STYLE_DASH_DOT:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				formatPenStyle = PS_DASHDOT;
				RECT rect;
				HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_PEN);
				GetClientRect(hControl, &rect);
				InvalidateRect(hControl, &rect, TRUE);
				UpdateWindow(hControl);
			}
			}
			break;
		}
		case IDC_PEN_STYLE_DASH_DOT_DOT:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				formatPenStyle = PS_DASHDOTDOT;
				RECT rect;
				HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_PEN);
				GetClientRect(hControl, &rect);
				InvalidateRect(hControl, &rect, TRUE);
				UpdateWindow(hControl);
			}
			}
			break;
		}
		case IDC_PEN_STYLE_IF:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				formatPenStyle = PS_INSIDEFRAME;
				RECT rect;
				HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_PEN);
				GetClientRect(hControl, &rect);
				InvalidateRect(hControl, &rect, TRUE);
				UpdateWindow(hControl);
			}
			}
			break;
		}
		case IDC_PEN_STYLE_NONE:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				formatPenStyle = PS_NULL;
				RECT rect;
				HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_PEN);
				GetClientRect(hControl, &rect);
				InvalidateRect(hControl, &rect, TRUE);
				UpdateWindow(hControl);
			}
			}
			break;
		}
		case IDOK:
		{
			tempWidth = formatPenWidth;
			tempColor = formatPenColor;
			tempPenStyle = formatPenStyle;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}
		
		break;
	}
	return (INT_PTR)FALSE;
}




// Message handler for brush box.
INT_PTR CALLBACK Brush(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		formatBrushIsFill = tempIsFill;
		formatBrush.lbColor = tempFill;
		formatBrush.lbHatch = HS_BDIAGONAL;
		formatBrush.lbStyle = BS_HATCHED;

		return (INT_PTR)TRUE;
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (pDIS->CtlID == IDC_PREVIEW_BRUSH)
		{
			RECT rcClient;
			GetClientRect(hDlg, &rcClient);
			FillRect(pDIS->hDC, &rcClient, (HBRUSH)(COLOR_WINDOW));
			
			HPEN hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
			SelectObject(pDIS->hDC, hPen);
			HBRUSH hBrush = NULL;
			if (formatBrushIsFill) {
				switch (formatBrush.lbStyle) {
				case BS_HATCHED:
					hBrush = CreateHatchBrush(formatBrush.lbHatch, formatBrush.lbColor);
					SelectObject(pDIS->hDC, hBrush);
					break;
				case BS_SOLID:
					hBrush = CreateSolidBrush(formatBrush.lbColor);
					SelectObject(pDIS->hDC, hBrush);
					break;
				case BS_NULL:
					SelectObject(pDIS->hDC, GetStockObject(NULL_BRUSH));
					break;
				}
			}
			else {
				SelectObject(pDIS->hDC, GetStockObject(NULL_BRUSH));
			}

			Ellipse(pDIS->hDC,
				4, 4,
				70, 70);
			DeleteObject(hBrush);
			DeleteObject(hPen);
		}
		else if (pDIS->CtlID == IDC_PREVIEW_BRUSH_COLOR) {
			RECT rcClient;
			GetClientRect(hDlg, &rcClient);
			FillRect(pDIS->hDC, &rcClient, (HBRUSH)(COLOR_WINDOW));
			SelectObject(pDIS->hDC, GetStockObject(DC_BRUSH));
			SetDCBrushColor(pDIS->hDC, formatBrush.lbColor);
			Rectangle(pDIS->hDC,
				0, 0,
				70, 30);
		}
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_COLORPICKER:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				COLORREF tempC = formatBrush.lbColor;
				ColorPickerDialog(hDlg, formatBrush.lbColor);
				if (!formatBrushIsFill) {
					if (tempC != formatBrush.lbColor)
						formatBrushIsFill = true;
					else
						formatBrushIsFill = false;
				}
				RECT rect;
				HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_BRUSH_COLOR);
				GetClientRect(hControl, &rect);
				InvalidateRect(hControl, &rect, TRUE);
				UpdateWindow(hControl);
			}
			}
			break;
		}
		case IDC_BRUSH_STYLE_SOLID:
		{
			formatBrush.lbStyle = BS_SOLID;
			RECT rect;
			HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_BRUSH);
			GetClientRect(hControl, &rect);
			InvalidateRect(hControl, &rect, TRUE);
			UpdateWindow(hControl);
			break;
		}
		case IDC_BRUSH_STYLE_HATCH:
		{
			formatBrush.lbStyle = BS_HATCHED;
			RECT rect;
			HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_BRUSH);
			GetClientRect(hControl, &rect);
			InvalidateRect(hControl, &rect, TRUE);
			UpdateWindow(hControl);
			break;
		}
		case IDC_BRUSH_STYLE_NONE:
		{
			formatBrush.lbStyle = BS_NULL;
			RECT rect;
			HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_BRUSH);
			GetClientRect(hControl, &rect);
			InvalidateRect(hControl, &rect, TRUE);
			UpdateWindow(hControl);
			break;
		}
		case IDC_BRUSH_HATCH_VERTICAL:
		{
			formatBrush.lbHatch = HS_VERTICAL;
			RECT rect;
			HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_BRUSH);
			GetClientRect(hControl, &rect);
			InvalidateRect(hControl, &rect, TRUE);
			UpdateWindow(hControl);
			break;
		}
		case IDC_BRUSH_HATCH_HORIZONTAL:
		{
			formatBrush.lbHatch = HS_HORIZONTAL;
			RECT rect;
			HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_BRUSH);
			GetClientRect(hControl, &rect);
			InvalidateRect(hControl, &rect, TRUE);
			UpdateWindow(hControl);
			break;
		}
		case IDC_BRUSH_HATCH_BDIAGONAL:
		{
			formatBrush.lbHatch = HS_BDIAGONAL;
			RECT rect;
			HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_BRUSH);
			GetClientRect(hControl, &rect);
			InvalidateRect(hControl, &rect, TRUE);
			UpdateWindow(hControl);
			break;
		}
		case IDC_BRUSH_HATCH_FDIAGONAL:
		{
			formatBrush.lbHatch = HS_FDIAGONAL;
			RECT rect;
			HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_BRUSH);
			GetClientRect(hControl, &rect);
			InvalidateRect(hControl, &rect, TRUE);
			UpdateWindow(hControl);
			break;
		}
		case IDC_BRUSH_HATCH_CROSS:
		{
			formatBrush.lbHatch = HS_CROSS;
			RECT rect;
			HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_BRUSH);
			GetClientRect(hControl, &rect);
			InvalidateRect(hControl, &rect, TRUE);
			UpdateWindow(hControl);
			break;
		}
		case IDC_BRUSH_HATCH_DIAGCROSS:
		{
			formatBrush.lbHatch = HS_DIAGCROSS;
			RECT rect;
			HWND hControl = GetDlgItem(hDlg, IDC_PREVIEW_BRUSH);
			GetClientRect(hControl, &rect);
			InvalidateRect(hControl, &rect, TRUE);
			UpdateWindow(hControl);
			break;
		}
		case IDOK:
		{
			tempIsFill = formatBrushIsFill;
			lb = formatBrush;

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}

		break;
	}
	return (INT_PTR)FALSE;
}


VOID ColorPickerDialog(HWND hWnd, COLORREF &color)
{
	//MSDN example
	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = color;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE)
	{
		color = cc.rgbResult;
		InvalidateRect(hWnd, NULL, true);
	}

}