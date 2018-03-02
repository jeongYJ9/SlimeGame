#pragma once

struct Point
{
	Point();
	Point(const Point& point);
	Point(const Size& size);
	Point(float x, float y);
	Point(const D3DXVECTOR2& vec);
	~Point();

	Point operator+(const Point& point) const;
	Point operator-(const Point& point) const;
	Point operator*(const Point& point) const;
	Point operator*(const float& amount) const;
	Point operator/(const Point& point) const;
	Point operator/(const float& amount) const;

	bool Equals(const Point& point) const;

	D3DXVECTOR2 ToVector2();

	float x;
	float y;
};