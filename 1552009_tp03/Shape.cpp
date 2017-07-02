#include "stdafx.h"
#include "Shape.h"

Shape * Shape::factory(int method)
{
	if (method == DRAW_LINE)
		return new MLine;
	else if (method == DRAW_RECT)
		return new MRectangle;
	else if (method == DRAW_ELLIPSE)
		return new MEllipse;
	else if (method == DRAW_RANDOM)
		return new MRandom;
	else if (method == DRAW_SQUARE)
		return new MSquare;
	else if (method == DRAW_ROUND)
		return new MRound;
	else if (method == DRAW_IMAGE)
		return new MImage;
	else
		return NULL;
}

void Shape::setColor(COLORREF strokeColor)
{
	this->color = strokeColor;
}

COLORREF Shape::getColor()
{
	return this->color;
}

void Shape::setStyle(DWORD strokeStyle)
{
	this->penStyle = strokeStyle;
}

void Shape::setWidth(DWORD strokeWidth)
{
	this->width = strokeWidth;
}

void Shape::setHidden()
{
	this->hidden = true;
}

void Shape::setVisible()
{
	this->hidden = false;
}

bool Shape::isHidden()
{
	return this->hidden;
}

int Shape::numPoint()
{
	return this->arrPoint.size();
}

void Shape::setBack(POINT a)
{
	this->arrPoint.back() = a;
}

void Shape::move(long diffX, long diffY)
{
	for (int i = 0; i < this->arrPoint.size(); i++) {
		this->arrPoint[i].x += diffX;
		this->arrPoint[i].y += diffY;
	}
}

void Shape::scale(long diffX, long diffY)
{
	if (this->arrPoint.size() < 2) 
		return;

	if (this->type == DRAW_RANDOM) {
		double ddiff = sqrt(diffX * diffX + diffY * diffY);
		POINT first;
		first.x = this->arrPoint[0].x;
		first.y = this->arrPoint[0].y;

		for (int i = 0; i < this->arrPoint.size(); i++) {
			if (diffX * diffY > 0) {
				if (diffX < 0) {
					this->arrPoint[i].x = this->arrPoint[i].x / ddiff;
					this->arrPoint[i].y = this->arrPoint[i].y / ddiff;
				}
				else {
					this->arrPoint[i].x = this->arrPoint[i].x * ddiff;
					this->arrPoint[i].y = this->arrPoint[i].y * ddiff;
				}
			}
		}

		POINT temp;
		temp.x = first.x - this->arrPoint[0].x;
		temp.y = first.y - this->arrPoint[0].y;

		for (int i = 0; i < this->arrPoint.size(); i++) {

			this->arrPoint[i].x = this->arrPoint[i].x + temp.x;
			this->arrPoint[i].y = this->arrPoint[i].y + temp.y;
		}
	}
	else {
		this->arrPoint[1].x += diffX;
		this->arrPoint[1].y += diffY;
	}
}

void Shape::addPoint(POINT a)
{
	this->arrPoint.push_back(a);
}

bool Shape::isSelected(RECT zone)
{
	RECT limit = { 0,0,0,0 };
	limit.bottom = limit.top = this->arrPoint[0].y;
	limit.right = limit.left = this->arrPoint[0].x;
	for (int i = 0; i < this->arrPoint.size(); i++)
	{
		if (this->arrPoint[i].x > limit.right)
			limit.right = this->arrPoint[i].x;
		if (this->arrPoint[i].x < limit.left)
			limit.left = this->arrPoint[i].x;
		if (this->arrPoint[i].y > limit.bottom)
			limit.bottom = this->arrPoint[i].y;
		if (this->arrPoint[i].y < limit.top)
			limit.top = this->arrPoint[i].y;
	}

	// select inside
	if (zone.bottom < limit.bottom
		&& zone.top > limit.top
		&& zone.left > limit.left 
		&& zone.right < limit.right)
		return true;
	
	// select outside
	if (zone.bottom > limit.bottom
		&& zone.top < limit.top
		&& zone.left < limit.left
		&& zone.right > limit.right)
		return true;

	return false;
}


MRectangle::MRectangle()
{
	this->isFill = false;
}

bool MRectangle::checkFill()
{
	return this->isFill;
}


void MRectangle::setIsFill(bool IsFill)
{
	this->isFill = IsFill;
}

void MRectangle::setBrush(LOGBRUSH Brush)
{
	this->brush = Brush;
}

COLORREF MRectangle::getColorFill()
{
	return this->brush.lbColor;
}

LOGBRUSH MRectangle::getBrush()
{
	return this->brush;
}

void MRectangle::setNoneFill()
{
	this->isFill = false;
}

void MRectangle::MouseMove(POINT a)
{
	if (this->arrPoint.size() < 2) {
		this->arrPoint.push_back(a);
	}
	else {
		this->arrPoint.back() = a;
	}
}

void MRectangle::MouseUp(POINT a)
{
	if (this->arrPoint.size() % 2 == 0) {
		this->arrPoint.back() = a;
	}
	else if (this->arrPoint.size() % 2 != 0) {
		this->arrPoint.push_back(a);
	}
}

void MRectangle::setFilename(LPWSTR)
{
}

LPWSTR MRectangle::getFilename()
{
	return LPWSTR();
}

void MRectangle::draw(HDC buffDC)
{
	if (this->isHidden()) return;
	LOGBRUSH lb;
	lb.lbColor = this->color;
	lb.lbHatch = NULL;
	lb.lbStyle = BS_SOLID;
	HPEN hPen = ExtCreatePen(PS_GEOMETRIC | this->penStyle, this->width, &lb, 0, NULL);
	SelectObject(buffDC, hPen);
	
	HBRUSH hBrush = NULL;
	if (this->isFill) {
		switch (this->brush.lbStyle) {
		case BS_HATCHED:
			hBrush = CreateHatchBrush(this->brush.lbHatch, this->brush.lbColor);
			SelectObject(buffDC, hBrush);
			break;
		case BS_SOLID:
			hBrush = CreateSolidBrush(this->brush.lbColor);
			SelectObject(buffDC, hBrush);
			break;
		case BS_NULL:
			SelectObject(buffDC, GetStockObject(NULL_BRUSH));
			break;
		}
	}
	else {
		SelectObject(buffDC, GetStockObject(NULL_BRUSH));
	}

	Rectangle(buffDC,
		(int)this->arrPoint[0].x, (int)this->arrPoint[0].y,
		(int)this->arrPoint[1].x, (int)this->arrPoint[1].y);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

bool MRandom::checkFill()
{
	return false;
}

void MRandom::setIsFill(bool)
{
}

COLORREF MRandom::getColorFill()
{
	return NULL;
}

LOGBRUSH MRandom::getBrush()
{
	return LOGBRUSH();
}


void MRandom::setBrush(LOGBRUSH)
{
}

void MRandom::setNoneFill()
{
}

void MRandom::MouseMove(POINT a)
{
	this->arrPoint.push_back(a);
}

void MRandom::MouseUp(POINT)
{
}

void MRandom::setFilename(LPWSTR)
{
}

LPWSTR MRandom::getFilename()
{
	return LPWSTR();
}

void MRandom::draw(HDC buffDC)
{
	if (this->isHidden()) return;
	LOGBRUSH lb;
	lb.lbColor = this->color;
	lb.lbStyle = BS_SOLID;
	lb.lbHatch = NULL;
	HPEN hPen = ExtCreatePen(PS_GEOMETRIC | this->penStyle, this->width, &lb, 0, NULL);
	SelectObject(buffDC, hPen);
	SelectObject(buffDC, GetStockObject(NULL_BRUSH));

	for (int j = 0; j < this->arrPoint.size() - 1; j++) {
		MoveToEx(buffDC, (int)this->arrPoint[j].x, (int)this->arrPoint[j].y, NULL);
		LineTo(buffDC, (int)this->arrPoint[j + 1].x, (int)this->arrPoint[j + 1].y);
	}

	DeleteObject(hPen);
}

bool MLine::checkFill()
{
	return false;
}

void MLine::setIsFill(bool)
{
}

COLORREF MLine::getColorFill()
{
	return NULL;
}

LOGBRUSH MLine::getBrush()
{
	return LOGBRUSH();
}


void MLine::setBrush(LOGBRUSH)
{
}

void MLine::setNoneFill()
{
}

void MLine::MouseMove(POINT a)
{
	if (this->arrPoint.size() < 2) {
		this->arrPoint.push_back(a);
	}
	else {
		this->arrPoint.back() = a;
	}
}

void MLine::MouseUp(POINT a)
{
	if (this->arrPoint.size() % 2 == 0) {
		this->arrPoint.back() = a;
	}
	else if (this->arrPoint.size() % 2 != 0) {
		this->arrPoint.push_back(a);
	}
}

void MLine::setFilename(LPWSTR)
{
}

LPWSTR MLine::getFilename()
{
	return LPWSTR();
}

void MLine::draw(HDC buffDC)
{
	if (this->isHidden()) return;
	LOGBRUSH lb;
	lb.lbColor = this->color;
	lb.lbStyle = BS_SOLID;
	lb.lbHatch = NULL;
	HPEN hPen = ExtCreatePen(PS_GEOMETRIC | this->penStyle, this->width, &lb, 0, NULL);
	SelectObject(buffDC, hPen);
	SelectObject(buffDC, GetStockObject(NULL_BRUSH));

	MoveToEx(buffDC, (int)this->arrPoint[0].x, (int)this->arrPoint[0].y, NULL);
	LineTo(buffDC, (int)this->arrPoint[1].x, (int)this->arrPoint[1].y);

	DeleteObject(hPen);
}

MEllipse::MEllipse()
{
	this->isFill = false;
}

bool MEllipse::checkFill()
{
	return this->isFill;
}

void MEllipse::setIsFill(bool IsFill)
{
	this->isFill = IsFill;
}

COLORREF MEllipse::getColorFill()
{
	return this->brush.lbColor;
}

LOGBRUSH MEllipse::getBrush()
{
	return this->brush;
}


void MEllipse::setBrush(LOGBRUSH Brush)
{
	this->brush = Brush;
}

void MEllipse::setNoneFill()
{
	this->isFill = false;
}

void MEllipse::MouseMove(POINT a)
{
	if (this->arrPoint.size() < 2) {
		this->arrPoint.push_back(a);
	}
	else {
		this->arrPoint.back() = a;
	}
}

void MEllipse::MouseUp(POINT a)
{
	if (this->arrPoint.size() % 2 == 0) {
		this->arrPoint.back() = a;
	}
	else if (this->arrPoint.size() % 2 != 0) {
		this->arrPoint.push_back(a);
	}
}

void MEllipse::setFilename(LPWSTR)
{
}

LPWSTR MEllipse::getFilename()
{
	return LPWSTR();
}

void MEllipse::draw(HDC buffDC)
{
	if (this->isHidden()) return;
	LOGBRUSH lb;
	lb.lbColor = this->color;
	lb.lbHatch = NULL;
	lb.lbStyle = BS_SOLID;
	HPEN hPen = ExtCreatePen(PS_GEOMETRIC | this->penStyle, this->width, &lb, 0, NULL);
	SelectObject(buffDC, hPen);

	HBRUSH hBrush = NULL;
	if (this->isFill) {
		switch (this->brush.lbStyle) {
		case BS_HATCHED:
			hBrush = CreateHatchBrush(this->brush.lbHatch, this->brush.lbColor);
			SelectObject(buffDC, hBrush);
			break;
		case BS_SOLID:
			hBrush = CreateSolidBrush(this->brush.lbColor);
			SelectObject(buffDC, hBrush);
			break;
		case BS_NULL:
			SelectObject(buffDC, GetStockObject(NULL_BRUSH));
			break;
		}
	}
	else {
		SelectObject(buffDC, GetStockObject(NULL_BRUSH));
	}

	Ellipse(buffDC,
		(int)this->arrPoint[0].x, (int)this->arrPoint[0].y,
		(int)this->arrPoint[1].x, (int)this->arrPoint[1].y);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

MSquare::MSquare()
{
	this->isFill = false;
}

bool MSquare::checkFill()
{
	return this->isFill;
}

void MSquare::setIsFill(bool IsFill)
{
	this->isFill = IsFill;
}

COLORREF MSquare::getColorFill()
{
	return this->brush.lbColor;
}

LOGBRUSH MSquare::getBrush()
{
	return this->brush;
}


void MSquare::setBrush(LOGBRUSH Brush)
{
	this->brush = Brush;
}

void MSquare::setNoneFill()
{
	this->isFill = false;
}

void MSquare::MouseMove(POINT a)
{
	if (this->arrPoint.size() < 2) {
		this->arrPoint.push_back(a);
	}
	else {
		this->arrPoint.back() = a;
	}
}

void MSquare::MouseUp(POINT a)
{
	if (this->arrPoint.size() % 2 == 0) {
		this->arrPoint.back() = a;
	}
	else if (this->arrPoint.size() % 2 != 0) {
		this->arrPoint.push_back(a);
	}
}

void MSquare::setFilename(LPWSTR)
{
}

LPWSTR MSquare::getFilename()
{
	return LPWSTR();
}

void MSquare::draw(HDC buffDC)
{
	if (this->isHidden()) return;
	LOGBRUSH lb;
	lb.lbColor = this->color;
	lb.lbHatch = NULL;
	lb.lbStyle = BS_SOLID;
	HPEN hPen = ExtCreatePen(PS_GEOMETRIC | this->penStyle, this->width, &lb, 0, NULL);
	SelectObject(buffDC, hPen);

	HBRUSH hBrush = NULL;
	if (this->isFill) {
		switch (this->brush.lbStyle) {
		case BS_HATCHED:
			hBrush = CreateHatchBrush(this->brush.lbHatch, this->brush.lbColor);
			SelectObject(buffDC, hBrush);
			break;
		case BS_SOLID:
			hBrush = CreateSolidBrush(this->brush.lbColor);
			SelectObject(buffDC, hBrush);
			break;
		case BS_NULL:
			SelectObject(buffDC, GetStockObject(NULL_BRUSH));
			break;
		}
	}
	else {
		SelectObject(buffDC, GetStockObject(NULL_BRUSH));
	}

	double d = sqrt((this->arrPoint[0].x - this->arrPoint[1].x) * (this->arrPoint[0].x - this->arrPoint[1].x) + 
		(this->arrPoint[0].y - this->arrPoint[1].y) * (this->arrPoint[0].y - this->arrPoint[1].y)) / 2;

	POINT mid;
	mid.x = (this->arrPoint[0].x + this->arrPoint[1].x) / 2;
	mid.y = (this->arrPoint[0].y + this->arrPoint[1].y) / 2;

	POINT vectorT;
	vectorT.x = - (this->arrPoint[0].y - this->arrPoint[1].y) / 2;
	vectorT.y = (this->arrPoint[0].x - this->arrPoint[1].x) / 2;

	POINT p1;
	p1.x = mid.x + (vectorT.x * d / sqrt(vectorT.x * vectorT.x + vectorT.y * vectorT.y));
	p1.y = mid.y + (vectorT.y * d / sqrt(vectorT.x * vectorT.x + vectorT.y * vectorT.y));
	POINT p2;
	p2.x = mid.x - (vectorT.x * d / sqrt(vectorT.x * vectorT.x + vectorT.y * vectorT.y));
	p2.y = mid.y - (vectorT.y * d / sqrt(vectorT.x * vectorT.x + vectorT.y * vectorT.y));

	POINT polygon[4];
	polygon[0] = this->arrPoint[0];
	polygon[1] = p1;
	polygon[2] = this->arrPoint[1];
	polygon[3] = p2;

	Polygon(buffDC, polygon, 4);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

MRound::MRound()
{
	this->isFill = false;
}

bool MRound::checkFill()
{
	return this->isFill;
}

void MRound::setIsFill(bool IsFill)
{
	this->isFill = IsFill;
}

COLORREF MRound::getColorFill()
{
	return this->brush.lbColor;
}

LOGBRUSH MRound::getBrush()
{
	return this->brush;
}


void MRound::setBrush(LOGBRUSH Brush)
{
	this->brush = Brush;
}

void MRound::setNoneFill()
{
	this->isFill = false;
}

void MRound::MouseMove(POINT a)
{
	if (this->arrPoint.size() < 2) {
		this->arrPoint.push_back(a);
	}
	else {
		this->arrPoint.back() = a;
	}
}

void MRound::MouseUp(POINT a)
{
	if (this->arrPoint.size() % 2 == 0) {
		this->arrPoint.back() = a;
	}
	else if (this->arrPoint.size() % 2 != 0) {
		this->arrPoint.push_back(a);
	}
}

void MRound::setFilename(LPWSTR)
{
}

LPWSTR MRound::getFilename()
{
	return LPWSTR();
}

void MRound::draw(HDC buffDC)
{
	if (this->isHidden()) return;
	LOGBRUSH lb;
	lb.lbColor = this->color;
	lb.lbHatch = NULL;
	lb.lbStyle = BS_SOLID;
	HPEN hPen = ExtCreatePen(PS_GEOMETRIC | this->penStyle, this->width, &lb, 0, NULL);
	SelectObject(buffDC, hPen);

	HBRUSH hBrush = NULL;
	if (this->isFill) {
		switch (this->brush.lbStyle) {
		case BS_HATCHED:
			hBrush = CreateHatchBrush(this->brush.lbHatch, this->brush.lbColor);
			SelectObject(buffDC, hBrush);
			break;
		case BS_SOLID:
			hBrush = CreateSolidBrush(this->brush.lbColor);
			SelectObject(buffDC, hBrush);
			break;
		case BS_NULL:
			SelectObject(buffDC, GetStockObject(NULL_BRUSH));
			break;
		}
	}
	else {
		SelectObject(buffDC, GetStockObject(NULL_BRUSH));
	}

	POINT vecT;
	vecT.x = this->arrPoint[1].x - this->arrPoint[0].x;
	vecT.y = this->arrPoint[1].y - this->arrPoint[0].y;

	if (abs(vecT.x) > abs(vecT.y))
		Ellipse(buffDC,
			(int)this->arrPoint[0].x, (int)this->arrPoint[0].y,
			(int)this->arrPoint[0].x + vecT.x, (int)this->arrPoint[0].y + abs(vecT.x) * (vecT.y >= 0 ? 1 : -1));
	else
		Ellipse(buffDC,
			(int)this->arrPoint[0].x, (int)this->arrPoint[0].y,
			(int)this->arrPoint[0].x + abs(vecT.y)  * (vecT.x >= 0 ? 1 : -1), (int)this->arrPoint[0].y + vecT.y);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void redraw(HWND hWnd, HDC hdc, std::vector<Shape*> Shapes)
{
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	FillRect(hdc, &rcClient, (HBRUSH)(COLOR_WINDOW + 1));

	for (int i = 0; i < Shapes.size(); i++) {
		Shapes[i]->draw(hdc);
	}
}

bool MImage::checkFill()
{
	return false;
}

void MImage::setIsFill(bool)
{
}

COLORREF MImage::getColorFill()
{
	return COLORREF();
}

LOGBRUSH MImage::getBrush()
{
	return LOGBRUSH();
}

void MImage::setBrush(LOGBRUSH)
{
}

void MImage::setNoneFill()
{
}

void MImage::MouseMove(POINT a)
{
	if (this->arrPoint.size() == 2) {
		this->arrPoint.back() = a;
	}
	else if (this->arrPoint.size() < 2) {
		this->arrPoint.push_back(a);
	}
}

void MImage::MouseUp(POINT a)
{
	if (this->arrPoint.size() % 2 == 0) {
		this->arrPoint.back() = a;
	}
	else if (this->arrPoint.size() % 2 != 0) {
		this->arrPoint.push_back(a);
	}
}

void MImage::setFilename(LPWSTR Filename)
{
	int i;
	for (i = 0; Filename[i]; i++) {
		this->filename[i] = Filename[i];
	}
	this->filename[i] = '\0';
}

LPWSTR MImage::getFilename()
{
	return this->filename;
}

void MImage::draw(HDC hdc)
{
	HDC memDC = CreateCompatibleDC(hdc);
	LPWSTR fn = this->filename;
	// bmp
	BITMAP bm;
	HANDLE hBmp, hOldBmp;
	hBmp = LoadImage(NULL, fn, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(hBmp, sizeof(BITMAP), &bm);

	hOldBmp = (HBITMAP)SelectObject(memDC, hBmp);
	StretchBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	SelectObject(hdc, hOldBmp);

	DeleteObject(hBmp);
	DeleteObject(memDC);
}

bool MText::checkFill()
{
	return false;
}

void MText::setIsFill(bool)
{
}

COLORREF MText::getColorFill()
{
	return COLORREF();
}

LOGBRUSH MText::getBrush()
{
	return LOGBRUSH();
}

void MText::setBrush(LOGBRUSH)
{
}

void MText::setNoneFill()
{
}

void MText::MouseMove(POINT)
{
}

void MText::MouseUp(POINT)
{
}

void MText::setFilename(LPWSTR)
{
}

LPWSTR MText::getFilename()
{
	return LPWSTR();
}

void MText::draw(HDC)
{
}
