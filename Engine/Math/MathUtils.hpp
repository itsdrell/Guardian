#pragma once
//====================================================================================
// Forward Declare
//====================================================================================
class Vector2;
class Vector3;

//====================================================================================
// Defines
//====================================================================================
#define PI 3.14159265359f

//====================================================================================
// Standalone C Functions
//====================================================================================
float	RangeMapFloat(float inValue, float inStart, float inEnd, float outStart, float outEnd);

float	ConvertRadiansToDegrees(float radians);
float	ConvertDegreesToRadians(float degrees);
float	CosDegrees(float degrees);
float	SinDegrees(float degrees);
float	ATan2fDegrees(float x, float y);

float	GetRandomFloat(float min, float max);
int		GetRandomIntRange(int minInclusive, int maxInclusive);
float	GetRandomFloatZeroToOne();
int		GetRandomIntLessThan(int maxNotInclusive);

bool	Chance(float chance);

float	Interpolate(float start, float end, float fractionTowardEnd);

float	ClampFloat(float currentValue, float min, float max);
int		ClampInt(int inValue, int min, int max);
float	GetFractionOf(float value);
float	SquareRoot(float value);

float	DotProduct(const Vector2& a, const Vector2& b);
float	GetDistance(const Vector2& a, const Vector2& b);
float	GetDistanceSquared(const Vector2& a, const Vector2& b);

Vector3 PolarToCartesian(float radius, float rotation, float azimuth);
Vector3 PolarToCartesian(const Vector3& spherical);