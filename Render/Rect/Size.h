#pragma once

struct Size
{
	Size();
	Size(const Size& size); //복사생성자
	Size(float width, float height);
	Size(const D3DXVECTOR2& vec);
	~Size();

	Size operator+(const Size& size) const;
	Size operator-(const Size& size) const;
	bool Equals(const Size& size) const;
	bool Empty() const;

	D3DXVECTOR2 ToVector2();

	float width;
	float height;
};