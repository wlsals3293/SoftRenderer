#include "Precompiled.h"
#include "Camera2D.h"


Matrix3x3 Camera2D::GetViewMatrix()
{
	float rad = Math::Deg2Rad(Transform.GetRotation());
	Matrix3x3 tMat = Matrix3x3(Vector3::UnitX, Vector3::UnitY, Vector3(-Transform.GetPosition().X, -Transform.GetPosition().Y, 1.f));
	Matrix3x3 rMat = Matrix3x3(Vector3(cosf(rad), -sinf(rad), 0.f), Vector3(sinf(rad), cosf(rad), 0.f), Vector3::UnitZ);

	return rMat * tMat;
}
