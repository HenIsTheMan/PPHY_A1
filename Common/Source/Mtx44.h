#pragma once
#include "Vector3.h"
#pragma warning(disable: 4290) //For throw(DivideByZero)

struct Mtx44 final{
	Mtx44(float = 0, float = 0, float = 0, float = 0, float = 0, float = 0, float = 0, float = 0, float = 0, float = 0, float = 0, float = 0, float = 0, float = 0, float = 0, float = 0);
	Mtx44(const float[16]);
	Mtx44(const Mtx44&);
	void SetToIdentity(), Transpose(), SetToZero();
	Mtx44 GetTranspose() const, GetInverse() const throw(DivideByZero), operator*(const Mtx44&) const, operator+(const Mtx44&) const, operator*(float) const;
	Mtx44 &operator=(const Mtx44&);
	Vector3 operator*(const Vector3&) const;
	void SetToRotation(float, float, float, float) throw(DivideByZero), SetToScale(float, float, float), SetToTranslation(float, float, float);
	void SetToFrustum(double, double, double, double, double, double);
	void SetToLookAt(double, double, double, double, double, double, double, double, double);
	void SetToPerspective(double, double, double, double), SetToOrtho(double, double, double, double, double, double);
	float a[16];
};