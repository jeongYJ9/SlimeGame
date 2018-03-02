#include "../../stdafx.h"
#include "Rect.h"

Rect::Rect()
{
	location = Point(0, 0);
	size = Size(0, 0);

	color = 0xFF000000;
}

Rect::Rect(const Rect & rect)
{
	location = rect.location;
	size = rect.size;
	color = rect.color;
}

Rect::Rect(float x, float y, float width, float height, DWORD color)
{
	location = Point(x, y);
	size = Size(width, height);

	this->color = color;
}

Rect::Rect(const Point & location, const Size & size, DWORD color)
{
	this->location = location;
	this->size = size;

	this->color = color;
}

Rect::Rect(const D3DXVECTOR2 & location, const D3DXVECTOR2 & size, DWORD color)
{
	this->location = Point(location);
	this->size = Size(size);

	this->color = color;
}

Rect::Rect(const RECT & rect, DWORD color)
{
	location.x = (float)rect.left;
	location.y = (float)rect.top;
	size.width = (float)(rect.right - rect.left);
	size.height = (float)(rect.bottom - rect.top);

	this->color = color;
}

Rect::~Rect()
{

}

void Rect::GetRECT(RECT * rect) const
{
	rect->left = (LONG)GetLeft();
	rect->top = (LONG)GetTop();
	rect->right = (LONG)GetRight();
	rect->bottom = (LONG)GetBottom();
}

void Rect::GetHalfLocation(Point * location) const
{
	location->x = this->location.x + (size.width * 0.5f);
	location->y = this->location.y + (size.height * 0.5f);
}

void Rect::GetHalfSize(Size * size) const
{
	size->width = this->size.width * 0.5f;
	size->height = this->size.height * 0.5f;
}

float Rect::GetLeft() const
{
	return location.x;
}

float Rect::GetTop() const
{
	return location.y;
}

float Rect::GetRight() const
{
	return location.x + size.width;
}

float Rect::GetBottom() const
{
	return location.y + size.height;
}

bool Rect::IsEmptyArea() const
{
	return (size.width < 1.0f) || (size.height < 1.0f);
}

bool Rect::Equals(const Rect & rect) const
{
	bool isCheck = true;
	isCheck &= location.Equals(rect.location);
	isCheck &= size.Equals(rect.size);

	return isCheck;
}

bool Rect::Intersect(const Rect & a, const Rect & b)
{
	bool isCheck = true;
	isCheck &= (a.GetLeft() < b.GetRight());
	isCheck &= (a.GetTop() < b.GetBottom());
	isCheck &= (a.GetRight() > b.GetLeft());
	isCheck &= (a.GetBottom() > b.GetTop());

	return isCheck;
}

bool Rect::Intersect(const Rect & a, const Rect & b, Rect * c)
{
	float left = max(a.GetLeft(), b.GetLeft());
	float top = max(a.GetTop(), b.GetTop());
	float right = min(a.GetRight(), b.GetRight());
	float bottom = min(a.GetBottom(), b.GetBottom());

	if (c != NULL)
	{
		c->location.x = left;
		c->location.y = top;
		c->size.width = right - left;
		c->size.height = bottom - top;
	}

	return c->IsEmptyArea() == false;
}

Rect::Direction Rect::IntersectDir(const Rect & a, const Rect & b)
{
	Point halfLocation1, halfLocation2;
	a.GetHalfLocation(&halfLocation1);
	b.GetHalfLocation(&halfLocation2);

	Size halfSize1, halfSize2;
	a.GetHalfSize(&halfSize1);
	b.GetHalfSize(&halfSize2);

	Point halfLocation = halfLocation1 - halfLocation2;
	Size halfSize = halfSize1 + halfSize2;

	Direction direction = None;
	if (fabs(halfLocation.x) < (double)halfSize.width || fabs(halfLocation.y) < (double)halfSize.height)
	{
		Size overlap;
		overlap.width = halfSize.width - (float)fabs(halfLocation.x);
		overlap.height = halfSize.height - (float)fabs(halfLocation.y);

		if (overlap.width >= overlap.height)
			return halfLocation.y > 0 ? Bottom : Top;
		else
			return halfLocation.x > 0 ? Right : Left;
	}

	return Direction::None;
}