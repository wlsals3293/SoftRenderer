#pragma once

#include "Vector3.h"


struct Matrix3x3
{
public:
	Matrix3x3();
	Matrix3x3(const Vector3& InCol0, const Vector3& InCol1, const Vector3& InCol2);
	Matrix3x3(float In00, float In01, float In02, float In10, float In11, float In12, float In20, float In21, float In22);

	FORCEINLINE void SetIdentity();
	FORCEINLINE void SetTranslation(float InX, float InY);
	FORCEINLINE void SetRotation(float InRadian);
	FORCEINLINE void SetScale(Vector3 InScale);
	FORCEINLINE Matrix3x3 Tranpose() const;

	FORCEINLINE const Vector3& operator[](int InIndex) const;
	FORCEINLINE Vector3& operator[](int InIndex);

	FORCEINLINE Matrix3x3 operator*(const Matrix3x3& InM) const;

	FORCEINLINE Vector3 operator*(const Vector3& InV) const;
	FORCEINLINE friend Vector3 operator*=(Vector3& InV, const Matrix3x3& InM)
	{
		InV = InM * InV;
		return InV;
	}

	FORCEINLINE Matrix3x3 operator*(float InS) const;

	static const Matrix3x3 Identity;

private:
	Vector3 Cols[3];
};

FORCEINLINE void Matrix3x3::SetIdentity()
{
	*this = Matrix3x3::Identity;
}

FORCEINLINE void Matrix3x3::SetTranslation(float InX, float InY)
{
	Cols[0] = Vector3::UnitX;
	Cols[1] = Vector3::UnitY;
	Cols[2] = Vector3::UnitZ;

	Cols[2].X = InX;
	Cols[2].Y = InY;
}

FORCEINLINE void Matrix3x3::SetRotation(float InRadian)
{
	Cols[0] = Vector3(cosf(InRadian), sinf(InRadian));
	Cols[1] = Vector3(-sinf(InRadian), cosf(InRadian));
}

FORCEINLINE void Matrix3x3::SetScale(Vector3 InScale)
{
	Cols[0] = Vector3::UnitX * InScale.X;
	Cols[1] = Vector3::UnitY * InScale.Y;
	Cols[2] = Vector3::UnitZ * InScale.Z;
}

FORCEINLINE Matrix3x3 Matrix3x3::Tranpose() const
{
	return Matrix3x3(
		Vector3(Cols[0].X, Cols[1].X, Cols[2].X),
		Vector3(Cols[0].Y, Cols[1].Y, Cols[2].Y),
		Vector3(Cols[0].Z, Cols[1].Z, Cols[2].Z)
	);
}


FORCEINLINE const Vector3& Matrix3x3::operator[](int InIndex) const
{
	return Cols[InIndex];
}

FORCEINLINE Vector3& Matrix3x3::operator[](int InIndex)
{
	return Cols[InIndex];
}


FORCEINLINE Matrix3x3 Matrix3x3::operator*(const Matrix3x3 &InM) const
{
	Matrix3x3 tpMat = Tranpose();
	return Matrix3x3(
		Vector3(tpMat[0].Dot(InM[0]), tpMat[1].Dot(InM[0]), tpMat[2].Dot(InM[0])),
		Vector3(tpMat[0].Dot(InM[1]), tpMat[1].Dot(InM[1]), tpMat[2].Dot(InM[1])),
		Vector3(tpMat[0].Dot(InM[2]), tpMat[1].Dot(InM[2]), tpMat[2].Dot(InM[2]))
	);
}

FORCEINLINE Vector3 Matrix3x3::operator*(const Vector3& InV) const
{
	Matrix3x3 tpMat = Tranpose();
	return Vector3(
		tpMat[0].Dot(InV),
		tpMat[1].Dot(InV),
		tpMat[2].Dot(InV)
	);
}

FORCEINLINE Matrix3x3 Matrix3x3::operator*(float InS) const
{
	return Matrix3x3(
		Cols[0] * InS,
		Cols[1] * InS,
		Cols[2] * InS
	);
}
