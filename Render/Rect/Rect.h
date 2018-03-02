#pragma once

struct Rect
{
public:
	Rect();
	Rect(const Rect& rect);
	Rect(float x, float y, float width, float height, DWORD color = 0xFF000000);
	Rect(const Point& location, const Size& size, DWORD color = 0xFF000000);
	Rect(const D3DXVECTOR2& location, const D3DXVECTOR2& size, DWORD color = 0xFF000000);
	Rect(const RECT& rect, DWORD color = 0xFF000000);
	~Rect();

	void GetRECT(RECT* rect) const;

	void GetHalfLocation(Point* location) const;
	void GetHalfSize(Size* size) const;

	float GetLeft() const;
	float GetTop() const;
	float GetRight() const;
	float GetBottom() const;

	bool IsEmptyArea() const;
	bool Equals(const Rect& rect) const;

	static bool Intersect(const Rect& a, const Rect& b);
	static bool Intersect(const Rect & a, const Rect & b, Rect * c);

	enum Direction { None = 0, Left, Right, Top, Bottom };
	static Direction IntersectDir(const Rect& a, const Rect& b);

public:
	Point location;
	Size size;

	DWORD color;
};