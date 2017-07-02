#pragma once
#include <Windows.h>
#include <vector>
#include <tchar.h>

#define DRAW_LINE		1
#define DRAW_RECT		2
#define DRAW_ELLIPSE	3
#define DRAW_RANDOM		4
#define DRAW_SQUARE		5
#define DRAW_ROUND		6
#define DRAW_IMAGE		7
#define DRAW_TEXT		8
#define CONTROL_DRAW	0
#define CONTROL_MOVE	1
#define CONTROL_SCALE	2
#define CONTROL_TEXT	3
#define CONTROL_SELECT	4
#define CONTROL_COPY	5
#define CONTROL_CUT		6
#define CONTROL_PASTE	7


class Shape {
public:
	int type;
	COLORREF color;
	bool hidden;
	DWORD penStyle;
	DWORD width;
	std::vector<POINT> arrPoint;

	static Shape* factory(int method);
	void setColor(COLORREF);
	COLORREF getColor();
	void setStyle(DWORD);
	void setWidth(DWORD);
	void setHidden();
	void setVisible();
	bool isHidden();
	int numPoint();
	void setBack(POINT);
	void move(long, long);
	void scale(long, long);
	void addPoint(POINT a);
	bool isSelected(RECT);
	virtual bool checkFill() = 0;
	virtual void setIsFill(bool) = 0;
	virtual void setBrush(LOGBRUSH) = 0;
	virtual COLORREF getColorFill() = 0;
	virtual LOGBRUSH getBrush() = 0;
	virtual void setNoneFill() = 0;
	virtual void MouseMove(POINT) = 0;
	virtual void MouseUp(POINT) = 0;
	virtual void setFilename(LPWSTR) = 0;
	virtual LPWSTR getFilename() = 0;
	virtual void setText(LPWSTR) = 0;
	virtual LPWSTR getText() = 0;
	virtual void setFont(HFONT) = 0;
	virtual HFONT getFont() = 0;
	virtual void draw(HDC buffDC) = 0;
};

class MRectangle : public Shape {
public:
	LOGBRUSH brush;
	bool isFill;

	MRectangle();
	bool checkFill();
	void setIsFill(bool);
	void setBrush(LOGBRUSH);
	COLORREF getColorFill();
	LOGBRUSH getBrush();
	void setNoneFill();
	void MouseMove(POINT);
	void MouseUp(POINT);
	void setFilename(LPWSTR);
	LPWSTR getFilename();
	void setText(LPWSTR);
	LPWSTR getText();
	void setFont(HFONT);
	HFONT getFont();
	void draw(HDC);
};

class MLine : public Shape {
public:
	bool checkFill();
	void setIsFill(bool);
	COLORREF getColorFill();
	LOGBRUSH getBrush();
	void setBrush(LOGBRUSH);
	void setNoneFill();
	void MouseMove(POINT);
	void MouseUp(POINT);
	void setFilename(LPWSTR);
	LPWSTR getFilename();
	void setText(LPWSTR);
	LPWSTR getText();
	void setFont(HFONT);
	HFONT getFont();
	void draw(HDC);
};

class MEllipse : public Shape {
public:
	LOGBRUSH brush;
	bool isFill;

	MEllipse();
	bool checkFill();
	void setIsFill(bool);
	COLORREF getColorFill();
	LOGBRUSH getBrush();
	void setBrush(LOGBRUSH);
	void setNoneFill();
	void MouseMove(POINT);
	void MouseUp(POINT);
	void setFilename(LPWSTR);
	LPWSTR getFilename();
	void setText(LPWSTR);
	LPWSTR getText();
	void setFont(HFONT);
	HFONT getFont();
	void draw(HDC);
};

class MRandom : public Shape {
public:
	bool checkFill();
	void setIsFill(bool);
	COLORREF getColorFill();
	LOGBRUSH getBrush();
	void setBrush(LOGBRUSH);
	void setNoneFill();
	void MouseMove(POINT);
	void MouseUp(POINT);
	void setFilename(LPWSTR);
	LPWSTR getFilename();
	void setText(LPWSTR);
	LPWSTR getText();
	void setFont(HFONT);
	HFONT getFont();
	void draw(HDC);
};

class MSquare : public Shape {
public:
	LOGBRUSH brush;
	bool isFill;

	MSquare();
	bool checkFill();
	void setIsFill(bool);
	COLORREF getColorFill();
	LOGBRUSH getBrush();
	void setBrush(LOGBRUSH);
	void setNoneFill();
	void MouseMove(POINT);
	void MouseUp(POINT);
	void setFilename(LPWSTR);
	LPWSTR getFilename();
	void setText(LPWSTR);
	LPWSTR getText();
	void setFont(HFONT);
	HFONT getFont();
	void draw(HDC);
};

class MRound : public Shape {
public:
	LOGBRUSH brush;
	bool isFill;

	MRound();
	bool checkFill();
	void setIsFill(bool);
	COLORREF getColorFill();
	LOGBRUSH getBrush();
	void setBrush(LOGBRUSH);
	void setNoneFill();
	void MouseMove(POINT);
	void MouseUp(POINT);
	void setFilename(LPWSTR);
	LPWSTR getFilename();
	void setText(LPWSTR);
	LPWSTR getText();
	void setFont(HFONT);
	HFONT getFont();
	void draw(HDC);
};

class MImage : public Shape {
public:
	TCHAR filename[260];

	bool checkFill();
	void setIsFill(bool);
	COLORREF getColorFill();
	LOGBRUSH getBrush();
	void setBrush(LOGBRUSH);
	void setNoneFill();
	void MouseMove(POINT);
	void MouseUp(POINT);
	void setFilename(LPWSTR);
	LPWSTR getFilename();
	void setText(LPWSTR);
	LPWSTR getText();
	void setFont(HFONT);
	HFONT getFont();
	void draw(HDC);
};

class MText : public Shape {
public:
	TCHAR text[260];
	HFONT font;

	bool checkFill();
	void setIsFill(bool);
	COLORREF getColorFill();
	LOGBRUSH getBrush();
	void setBrush(LOGBRUSH);
	void setNoneFill();
	void MouseMove(POINT);
	void MouseUp(POINT);
	void setFilename(LPWSTR);
	LPWSTR getFilename();
	void setText(LPWSTR);
	LPWSTR getText();
	void setFont(HFONT);
	HFONT getFont();
	void draw(HDC);
};

void redraw(HWND hWnd, HDC hdc, std::vector<Shape *> Shapes);