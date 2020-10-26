#include "Vector3.h"

Vector3::Vector3(){
	x = y = z = 0.f;
}

Vector3::Vector3(const float& xyz){
	x = y = z = xyz;
}

Vector3::Vector3(const float& a, const float& b, const float& c): x(a), y(b), z(c){}

Vector3::Vector3(const Vector3 &RHS) : x(RHS.x), y(RHS.y), z(RHS.z){}

bool Vector3::IsEqual(const float& a, const float& b) const{
	return a - b <= Math::EPSILON && b - a <= Math::EPSILON;
}

void Vector3::Set(const float& a, const float& b, const float& c){
	x = a;
	y = b;
	z = c;
}

void Vector3::SetZero(){
	x = y = z = 0.f;
}

bool Vector3::IsZero() const{
	return IsEqual(x, 0.f) && IsEqual(y, 0.f) && IsEqual(z, 0.f);
}

Vector3 Vector3::operator+(const Vector3 &RHS) const{
	return Vector3(x + RHS.x, y + RHS.y, z + RHS.z);
}

Vector3& Vector3::operator+=(const Vector3 &RHS){
	x += RHS.x;
	y += RHS.y;
	z += RHS.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3 &RHS) const{
	return Vector3(x - RHS.x, y - RHS.y, z - RHS.z);
}

Vector3& Vector3::operator-=(const Vector3& RHS){
	x -= RHS.x;
	y -= RHS.y;
	z -= RHS.z;
	return *this;
}

Vector3 Vector3::operator-() const{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator*(const float& scalar) const{
	return Vector3(scalar * x, scalar * y, scalar * z);
}

Vector3& Vector3::operator*=(const float& scalar){
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

bool Vector3::operator==(const Vector3 &RHS) const{ //Using Math::EPSILON
	return IsEqual(x, RHS.x) && IsEqual(y, RHS.y) && IsEqual(z, RHS.z);
}

bool Vector3::operator!=(const Vector3 &RHS) const{ //Using Math::EPSILON
	return !IsEqual(x, RHS.x) || !IsEqual(y, RHS.y) || !IsEqual(z, RHS.z);
}

Vector3& Vector3::operator=(const Vector3 &RHS){
	x = RHS.x;
	y = RHS.y;
	z = RHS.z;
	return *this;
}

float Vector3::Length() const{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::LengthSquared() const{
	return x * x + y * y + z * z;
}

float Vector3::Dot(const Vector3 &RHS) const{
	return x * RHS.x + y * RHS.y + z * RHS.z;
}

Vector3 Vector3::Cross(const Vector3 &RHS) const{
	return Vector3(y * RHS.z - z * RHS.y, z * RHS.x - x * RHS.z, x * RHS.y - y * RHS.x);
}

Vector3 Vector3::Normalized() const throw(DivideByZero){
	float d = Length();
	if(d <= Math::EPSILON && -d <= Math::EPSILON){
		throw DivideByZero();
	}
	return Vector3(x / d, y / d, z / d);
}

Vector3& Vector3::Normalize() throw(DivideByZero){
	float d = Length();
	if(d <= Math::EPSILON && -d <= Math::EPSILON){
		throw DivideByZero();
	}
	x /= d;
	y /= d;
	z /= d;
	return *this;
}

std::ostream& operator<<(std::ostream &os, Vector3 &RHS){
	os << "(" << RHS.x << ", " << RHS.y << ", " << RHS.z << ")";
	return os;
}

Vector3 operator*(const float& scalar, const Vector3 &RHS){
	return RHS * scalar;
}