#include "Engine/Math/Vectors/Vector2.hpp"

//===============================================================================================
const Vector2 Vector2::ZERO = Vector2(0.f, 0.f);
const Vector2 Vector2::ONE = Vector2(1.f, 1.f);

//===============================================================================================
Vector2::Vector2()
{
	x,y = 0;
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator+(const Vector2& vecToAdd) const
{
	return Vector2(vecToAdd.x + x, vecToAdd.y + y);
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator-(const Vector2& vecToSubtract) const
{
	float newX = this->x - vecToSubtract.x;
	float newY = this->y - vecToSubtract.y;

	return Vector2(newX, newY);
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator*(float uniformScale) const
{
	float newX = this->x * uniformScale;
	float newY = this->y * uniformScale;

	return Vector2(newX, newY);
}

//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator/(float inverseScale) const
{
	float scale = 1.0f / inverseScale;

	// 	float newX = this->x / inverseScale;
	// 	float newY = this->y / inverseScale;

	float newX = this->x * scale;
	float newY = this->y * scale;

	return Vector2(newX, newY);
}

//-----------------------------------------------------------------------------------------------
void Vector2::operator+=(const Vector2& vecToAdd)
{
	this->x = this->x + vecToAdd.x;
	this->y = this->y + vecToAdd.y;
}

//-----------------------------------------------------------------------------------------------
void Vector2::operator-=(const Vector2& vecToSubtract)
{
	this->x = this->x - vecToSubtract.x;
	this->y = this->y - vecToSubtract.y;
}

//-----------------------------------------------------------------------------------------------
void Vector2::operator*=(const float uniformScale)
{
	this->x = this->x * uniformScale;
	this->y = this->y * uniformScale;
}

//-----------------------------------------------------------------------------------------------
void Vector2::operator/=(const float uniformDivisor)
{
	// if its a constant its as cheap just to divide by it...?

	this->x = this->x / uniformDivisor;
	this->y = this->y / uniformDivisor;
}

//-----------------------------------------------------------------------------------------------
void Vector2::operator=(const Vector2& copyFrom)
{
	this->x = copyFrom.x;
	this->y = copyFrom.y;
}

//-----------------------------------------------------------------------------------------------
const Vector2 operator*(float uniformScale, const Vector2& vecToScale)
{
	float scale = uniformScale;

	float newX = vecToScale.x * scale;
	float newY = vecToScale.y * scale;

	return Vector2(newX, newY);
}

//-----------------------------------------------------------------------------------------------
bool Vector2::operator==(const Vector2& compare) const
{
	if (this->x == compare.x)
	{
		if (this->y == compare.y)
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
bool Vector2::operator!=(const Vector2& compare) const
{
	// I could just call == and do the same thing but just so it's written out

	if (this->x == compare.x)
	{
		if (this->y == compare.y)
		{
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
Vector2::Vector2(float theX, float theY)
{
	x = theX;
	y = theY;
}
