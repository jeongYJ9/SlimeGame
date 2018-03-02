#include "../../stdafx.h"
#include "Point.h"

Point::Point()
{
	x = y = 0.0f;
}

Point::Point(const Point & point)
{
	x = point.x;
	y = point.y;
}

Point::Point(const Size & size)
{
	x = size.width;
	y = size.height;
}

Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}

Point::Point(const D3DXVECTOR2& vec)
{
	x = vec.x;
	y = vec.y;
}

Point::~Point()
{
}

Point Point::operator+(const Point & point) const
{
	return Point(x + point.x, y + point.y);
}

Point Point::operator-(const Point & point) const
{
	return Point(x - point.x, y - point.y);
}

Point Point::operator*(const Point & point) const
{
	return Point(x * point.x, y * point.y);
}

Point Point::operator*(const float & amount) const
{
	return Point(x * amount, y * amount);
}

Point Point::operator/(const Point & point) const
{
	return Point(x / point.x, y / point.y);
}

Point Point::operator/(const float & amount) const
{
	return Point(x / amount, y / amount);
}

bool Point::Equals(const Point & point) const
{
	return (x == point.x) && (y == point.y);
}

D3DXVECTOR2 Point::ToVector2()
{
	return D3DXVECTOR2(x, y);
}
