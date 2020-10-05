#pragma once
#include "Engine/Math/Vectors/Vector2.hpp"
#include "Engine/Math/Vectors/Vector3.hpp"
#include "Engine/Math/Vectors/Vector4.hpp"

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
class Matrix44
{
public:
	Matrix44();
	explicit Matrix44( const float* sixteenValuesBasisMajor ); // float[16] array in order Ix, Iy...
	explicit Matrix44(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation = Vector3(0.f, 0.f, 0.f));
	explicit Matrix44(const Vector4& iBasis, const Vector4& jBasis, const Vector4& kBasis, const Vector4& translation);

public:
	void operator=(const Matrix44& copyOf);

public:
	void SetIdentity();
	void Append( const Matrix44& matrixToAppend ); // a.k.a. Concatenate (right-multiply)
	void Translate2D( const Vector2& translation );
	void Transpose();

public:
	static Matrix44 MakeRotationDegreesAroundZ(float rotationDegrees);
	static Matrix44 MakeRotationDegreesAroundY(float rotationDegrees);
	static Matrix44 MakeRotationDegreesAroundX(float rotationDegrees);
	static Matrix44 MakeRotationDegrees2D(float rotationDegreesAboutZ);
	static Matrix44 MakeOrtho2D( const Vector2& mins, const Vector2& maxs );
	static Matrix44 MakeTranslation2D(const Vector2& translation);
	static Matrix44 MakeTranslation3D(const Vector3& translation);
	static Matrix44 MakeOrtho3D( const Vector3& mins, const Vector3& maxs );
	static Matrix44 LookAt(const Vector3& pos, const Vector3& target, Vector3 up = Vector3::UP);
	static Matrix44 MakePerspectiveProjection(float fov_degrees, float aspect, float nz, float fz);
	static Matrix44 MakeTranposeOf(const Matrix44& matrix);

public:
	// Column Major (right handed) (squirrels way)
	// Ix	Jx	Kx	Tx
	// Iy	Jy	Ky	Ty
	// Iz	Jz	Kz	Tz
	// Iw	Jw	Kw	Tw

	float	Ix, Iy, Iz, Iw,   Jx, Jy, Jz, Jw,   Kx, Ky, Kz, Kw,   Tx, Ty, Tz, Tw; // i, j, k and translation
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/31/2019]
//====================================================================================