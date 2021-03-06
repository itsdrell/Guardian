#pragma once

//====================================================================================
// Forward Declare
//====================================================================================


//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class Vector2
{
public:
	Vector2();
	Vector2( float theX, float theY );

public:
	Vector2 operator-() const { return Vector2(-x, -y); }
	const Vector2 operator+(const Vector2& vecToAdd) const;
	const Vector2 operator-(const Vector2& vecToSubtract) const;	// vec2 - vec2
	const Vector2 operator*(float uniformScale) const;			// vec2 * float
	const Vector2 operator/(float inverseScale) const;			// vec2 / float
	void operator+=(const Vector2& vecToAdd);						// vec2 += vec2
	void operator-=(const Vector2& vecToSubtract);				// vec2 -= vec2
	void operator*=(const float uniformScale);					// vec2 *= float
	void operator/=(const float uniformDivisor);					// vec2 /= float
	void operator=(const Vector2& copyFrom);						// vec2 = vec2
	bool operator==(const Vector2& compare) const;				// vec2 == vec2
	bool operator!=(const Vector2& compare) const;				// vec2 != vec2

	friend const Vector2 operator*(float uniformScale, const Vector2& vecToScale);	// float * vec2

public:
	float x,y;

public:
	const static Vector2 ZERO;
	const static Vector2 ONE;
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/29/2019]
//====================================================================================