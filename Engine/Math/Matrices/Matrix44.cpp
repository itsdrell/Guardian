#include "Matrix44.hpp"
#include "Engine/Math/MathUtils.hpp"

#include <corecrt_math.h> // tanf

//===============================================================================================
Matrix44::Matrix44()
{
	this->SetIdentity();
}

//-----------------------------------------------------------------------------------------------
Matrix44::Matrix44(const float* sixteenValuesBasisMajor)
{
	Ix = sixteenValuesBasisMajor[0];
	Iy = sixteenValuesBasisMajor[1];
	Iz = sixteenValuesBasisMajor[2];
	Iw = sixteenValuesBasisMajor[3];

	Jx= sixteenValuesBasisMajor[4];
	Jy= sixteenValuesBasisMajor[5];
	Jz= sixteenValuesBasisMajor[6];
	Jw= sixteenValuesBasisMajor[7];

	Kx = sixteenValuesBasisMajor[8];
	Ky = sixteenValuesBasisMajor[9];
	Kz = sixteenValuesBasisMajor[10];
	Kw = sixteenValuesBasisMajor[11];

	Tx = sixteenValuesBasisMajor[12];
	Ty = sixteenValuesBasisMajor[13];
	Tz = sixteenValuesBasisMajor[14];
	Tw = sixteenValuesBasisMajor[15];
}

//-----------------------------------------------------------------------------------------------
Matrix44::Matrix44(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation /*= Vector3(0.f, 0.f, 0.f)*/)
{
	SetIdentity();

	Ix = iBasis.x;
	Iy = iBasis.y;
	Iz = iBasis.z;

	Jx = jBasis.x;
	Jy = jBasis.y;
	Jz = jBasis.z;

	Kx = kBasis.x;
	Ky = kBasis.y;
	Kz = kBasis.z;

	Tx = translation.x;
	Ty = translation.y;
	Tz = translation.z;
}

//-----------------------------------------------------------------------------------------------
Matrix44::Matrix44(const Vector4& iBasis, const Vector4& jBasis, const Vector4& kBasis, const Vector4& translation)
{
	Ix = iBasis.x;
	Iy = iBasis.y;
	Iz = iBasis.z;
	Iw = iBasis.w;

	Jx = jBasis.x;
	Jy = jBasis.y;
	Jz = jBasis.z;
	Jw = jBasis.w;

	Kx = kBasis.x;
	Ky = kBasis.y;
	Kz = kBasis.z;
	Kw = kBasis.w;

	Tx = translation.x;
	Ty = translation.y;
	Tz = translation.z;
	Tw = translation.w;
}

//-----------------------------------------------------------------------------------------------
void Matrix44::operator=(const Matrix44& copyOf)
{
	Ix = copyOf.Ix;
	Iy = copyOf.Iy;
	Iz = copyOf.Iz;
	Iw = copyOf.Iw;

	Jx = copyOf.Jx;
	Jy = copyOf.Jy;
	Jz = copyOf.Jz;
	Jw = copyOf.Jw;

	Kx = copyOf.Kx;
	Ky = copyOf.Ky;
	Kz = copyOf.Kz;
	Kw = copyOf.Kw;

	Tx = copyOf.Tx;
	Ty = copyOf.Ty;
	Tz = copyOf.Tz;
	Tw = copyOf.Tw;
}

//-----------------------------------------------------------------------------------------------
void Matrix44::SetIdentity()
{
	// Set the diagonals
	Ix = Jy = Kz = Tw = 1.0f;

	// Set the rest to 0
	Iy = Iz = Iw = 0.0f;
	Jx = Jz = Jw = 0.0f;
	Kx = Ky = Kw = 0.0f;
	Tx = Ty = Tz = 0.0f;
}

//-----------------------------------------------------------------------------------------------
void Matrix44::Append(const Matrix44& matrixToAppend)
{
	Matrix44 old = Matrix44(&Ix); // Use the constructor that takes the memory pointer

	Ix = (old.Ix * matrixToAppend.Ix) + (old.Jx * matrixToAppend.Iy) + (old.Kx * matrixToAppend.Iz) + (old.Tx * matrixToAppend.Iw);
	Iy = (old.Iy * matrixToAppend.Ix) + (old.Jy * matrixToAppend.Iy) + (old.Ky * matrixToAppend.Iz) + (old.Ty * matrixToAppend.Iw);
	Iz = (old.Iz * matrixToAppend.Ix) + (old.Jz * matrixToAppend.Iy) + (old.Kz * matrixToAppend.Iz) + (old.Tz * matrixToAppend.Iw);
	Iw = (old.Iw * matrixToAppend.Ix) + (old.Jw * matrixToAppend.Iy) + (old.Kw * matrixToAppend.Iz) + (old.Tw * matrixToAppend.Iw);

	Jx = (old.Ix * matrixToAppend.Jx) + (old.Jx * matrixToAppend.Jy) + (old.Kx * matrixToAppend.Jz) + (old.Tx * matrixToAppend.Jw);
	Jy = (old.Iy * matrixToAppend.Jx) + (old.Jy * matrixToAppend.Jy) + (old.Ky * matrixToAppend.Jz) + (old.Ty * matrixToAppend.Jw);
	Jz = (old.Iz * matrixToAppend.Jx) + (old.Jz * matrixToAppend.Jy) + (old.Kz * matrixToAppend.Jz) + (old.Tz * matrixToAppend.Jw);
	Jw = (old.Iw * matrixToAppend.Jx) + (old.Jw * matrixToAppend.Jy) + (old.Kw * matrixToAppend.Jz) + (old.Tw * matrixToAppend.Jw);

	Kx = (old.Ix * matrixToAppend.Kx) + (old.Jx * matrixToAppend.Ky) + (old.Kx * matrixToAppend.Kz) + (old.Tx * matrixToAppend.Kw);
	Ky = (old.Iy * matrixToAppend.Kx) + (old.Jy * matrixToAppend.Ky) + (old.Ky * matrixToAppend.Kz) + (old.Ty * matrixToAppend.Kw);
	Kz = (old.Iz * matrixToAppend.Kx) + (old.Jz * matrixToAppend.Ky) + (old.Kz * matrixToAppend.Kz) + (old.Tz * matrixToAppend.Kw);
	Kw = (old.Iw * matrixToAppend.Kx) + (old.Jw * matrixToAppend.Ky) + (old.Kw * matrixToAppend.Kz) + (old.Tw * matrixToAppend.Kw);

	Tx = (old.Ix * matrixToAppend.Tx) + (old.Jx * matrixToAppend.Ty) + (old.Kx * matrixToAppend.Tz) + (old.Tx * matrixToAppend.Tw);
	Ty = (old.Iy * matrixToAppend.Tx) + (old.Jy * matrixToAppend.Ty) + (old.Ky * matrixToAppend.Tz) + (old.Ty * matrixToAppend.Tw);
	Tz = (old.Iz * matrixToAppend.Tx) + (old.Jz * matrixToAppend.Ty) + (old.Kz * matrixToAppend.Tz) + (old.Tz * matrixToAppend.Tw);
	Tw = (old.Iw * matrixToAppend.Tx) + (old.Jw * matrixToAppend.Ty) + (old.Kw * matrixToAppend.Tz) + (old.Tw * matrixToAppend.Tw);
}

//-----------------------------------------------------------------------------------------------
void Matrix44::Translate2D(const Vector2& translation)
{
	Matrix44 translationMatrix = MakeTranslation2D(translation);
	Append(translationMatrix);
}

//-----------------------------------------------------------------------------------------------
// if called first, this becomes row major
void Matrix44::Transpose()
{
	Matrix44 matrixToInverse = Matrix44(*this);

	//Iy = matrixToInverse.Jx;
	//Iz = matrixToInverse.Kx;
	//
	//Jx = matrixToInverse.Iy;
	//Jz = matrixToInverse.Ky;
	//
	//Kx = matrixToInverse.Iz;
	//Ky = matrixToInverse.Jz;

	Iy = matrixToInverse.Jx;
	Jx = matrixToInverse.Iy;

	Iz = matrixToInverse.Kx;
	Kx = matrixToInverse.Iz;

	Iw = matrixToInverse.Tx;
	Tx = matrixToInverse.Iw;

	Jz = matrixToInverse.Ky;
	Ky = matrixToInverse.Jz;

	Jw = matrixToInverse.Ty;
	Ty = matrixToInverse.Jw;

	Kw = matrixToInverse.Tz;
	Tz = matrixToInverse.Kw;
}

//-----------------------------------------------------------------------------------------------
Vector3 Matrix44::GetForward() const
{
	return Vector3(Kx, Ky, Kz);
}

//-----------------------------------------------------------------------------------------------
Vector3 Matrix44::GetRight() const
{
	return Vector3(Ix, Iy, Iz);
}

//-----------------------------------------------------------------------------------------------
Vector3 Matrix44::GetUp() const
{
	return Vector3(Jx, Jy, Jz);
}

//-----------------------------------------------------------------------------------------------
Vector3 Matrix44::GetPosition() const
{
	return Vector3(Tx, Ty, Tz);
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeRotationDegreesAroundZ(float rotationDegrees)
{
	return MakeRotationDegrees2D(rotationDegrees);
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeRotationDegreesAroundY(float rotationDegrees)
{
	//https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions

	Matrix44 result;

	float cosValue = CosDegrees(rotationDegrees);
	float sinValue = SinDegrees(rotationDegrees);

	result.Ix = cosValue;
	result.Iz = -sinValue;
	result.Kx = sinValue;
	result.Kz = cosValue;

	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeRotationDegreesAroundX(float rotationDegrees)
{
	// https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
	Matrix44 result;

	float cosValue = CosDegrees(rotationDegrees);
	float sinValue = SinDegrees(rotationDegrees);

	result.Jy = cosValue;
	result.Jz = sinValue;
	result.Ky = -sinValue;
	result.Kz = cosValue;

	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeRotationDegrees2D(float rotationDegreesAboutZ)
{
	Matrix44 result;

	float cosValue = CosDegrees(rotationDegreesAboutZ);
	float sinValue = SinDegrees(rotationDegreesAboutZ);

	result.Ix = cosValue;
	result.Iy = sinValue;
	result.Jx = -sinValue;
	result.Jy = cosValue;

	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeOrtho2D(const Vector2 & mins, const Vector2 & maxs)
{
	Matrix44 result = Matrix44(); // Get Identity 

	// Setting the diagonal
	result.Ix = (2)/(maxs.x - mins.x);
	result.Jy = (2)/(maxs.y - mins.y);

	// Set the T column
	result.Tx = -(maxs.x + mins.x) / (maxs.x - mins.x);
	result.Ty = -(maxs.y + mins.y) / (maxs.y - mins.y);
	result.Tw = 1.f;

	// The rest are all zeros
	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeTranslation2D(const Vector2& translation)
{
	Matrix44 result;
	result.Tx = translation.x;
	result.Ty = translation.y;

	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeTranslation3D(const Vector3& translation)
{
	Matrix44 result;

	result.Tx = translation.x;
	result.Ty = translation.y;
	result.Tz = translation.z;

	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeOrtho3D(const Vector3& mins, const Vector3& maxs)
{
	Matrix44 result = Matrix44(); // Get Identity 

	// Setting the diagonal
	result.Ix = (2)/(maxs.x - mins.x);
	result.Jy = (2)/(maxs.y - mins.y);
	result.Kz = (2)/(maxs.z - mins.z);

	// Set the T column
	result.Tx = -(maxs.x + mins.x) / (maxs.x - mins.x);
	result.Ty = -(maxs.y + mins.y) / (maxs.y - mins.y);
	result.Tz = -(maxs.z + mins.z) / (maxs.z - mins.z);
	result.Tw = 1.f;

	// The rest are all zeros
	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::LookAt(const Vector3& pos, const Vector3& target, Vector3 up /*= Vector3(0.f, 1.f, 0.f)*/)
{
	Vector3 direction = target - pos;
	Vector3 forward = Normalize(direction); // direction.Normalize();
	Vector3 right = Cross(up, forward);
	Vector3 normalizeRight = Normalize(right); //right.Normalize();
	Vector3 newUp = Cross(forward, normalizeRight);

	// this may be wrong
	Vector3 targetPos = Vector3(-pos.x, -pos.y, -pos.z);

	// Create a matrix
	return Matrix44(normalizeRight, newUp, forward, targetPos);
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakePerspectiveProjection(float fov_degrees, float aspect, float nz, float fz)
{
	float d = 1.0f / tanf(ConvertDegreesToRadians(fov_degrees));
	float q = 1.0f / (fz - nz);

	Vector4 i = Vector4(d / aspect, 0, 0, 0);
	Vector4 j = Vector4(0, d, 0, 0);
	Vector4 k = Vector4(0, 0, (nz + fz) * q, 1.f);
	Vector4 t = Vector4(0, 0, -2.0f * nz * fz * q, 0);

	Matrix44 result = Matrix44(i, j, k, t);

	return result;
}

//-----------------------------------------------------------------------------------------------
Matrix44 Matrix44::MakeTranposeOf(const Matrix44& matrix)
{
	Matrix44 result = matrix;
	result.Transpose();

	return result;
}
