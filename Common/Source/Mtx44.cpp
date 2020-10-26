#include "Mtx44.h"

Mtx44::Mtx44(float a00, float a10, float a20, float a30, float a01, float a11, float a21, float a31, float a02, float a12, float a22, float a32, float a03, float a13, float a23, float a33):
	a{a00, a10, a20, a30, a01, a11, a21, a31, a02, a12, a22, a32, a03, a13, a23, a33}{ //Default constructor, default values are zero, params (Initial values for the matrix in row-column format)
}

Mtx44::Mtx44(const float m[16]) : a{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}{ //Default values are zero, param m[16] (Initial values for the matrix in column major format)
	for(int i = 0; i < 16; ++i){
		a[i] = m[i];
	}
}

Mtx44::Mtx44(const Mtx44 &RHS): a{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}{ //Copy constructor
	for(int i = 0; i < 16; ++i){
		a[i] = RHS.a[i];
	}
}

void Mtx44::SetToIdentity(){
	SetToZero();
	a[0] = a[5] = a[10] = a[15] = 1;
}

void swap(float &v1, float &v2){ //Helper function swap
	v1 += v2;
	v2 = v1 - v2;
	v1 -= v2;
}

void Mtx44::Transpose(){
	for(int i = 0; i < 4; ++i){
		for(int j = i + 1; j < 4; ++j){
			swap(a[i * 4 + j], a[j * 4 + i]);
		}
	}
}

void Mtx44::SetToZero(){ //Set this matrix to zero
	for(int i = 0; i < 16; ++i){
		a[i] = 0;
	}
}

Mtx44 Mtx44::GetTranspose() const{
	Mtx44 ret(*this);
	ret.Transpose();
	return ret;
}

Mtx44 Mtx44::GetInverse() const throw(DivideByZero){ //Exception thrown if the determinant of the matrix is zero
	float a0 = a[0] * a[5] - a[1] * a[4], a1 = a[0] * a[6] - a[2] * a[4];
	float a2 = a[0] * a[7] - a[3] * a[4], a3 = a[1] * a[6] - a[2] * a[5];
	float a4 = a[1] * a[7] - a[3] * a[5], a5 = a[2] * a[7] - a[3] * a[6];
	float b0 = a[8] * a[13] - a[9] * a[12], b1 = a[8] * a[14] - a[10] * a[12];
	float b2 = a[8] * a[15] - a[11] * a[12], b3 = a[9] * a[14] - a[10] * a[13];
	float b4 = a[9] * a[15] - a[11] * a[13], b5 = a[10] * a[15] - a[11] * a[14];
	float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
	if(abs(det) < Math::EPSILON){
		throw DivideByZero();
	}
	Mtx44 inverse;
	if(Math::FAbs(det) > Math::EPSILON){
		inverse.a[0] = +a[5] * b5 - a[6] * b4 + a[7] * b3;
		inverse.a[4] = -a[4] * b5 + a[6] * b2 - a[7] * b1;
		inverse.a[8] = +a[4] * b4 - a[5] * b2 + a[7] * b0;
		inverse.a[12] = -a[4] * b3 + a[5] * b1 - a[6] * b0;
		inverse.a[1] = -a[1] * b5 + a[2] * b4 - a[3] * b3;
		inverse.a[5] = +a[0] * b5 - a[2] * b2 + a[3] * b1;
		inverse.a[9] = -a[0] * b4 + a[1] * b2 - a[3] * b0;
		inverse.a[13] = +a[0] * b3 - a[1] * b1 + a[2] * b0;
		inverse.a[2] = +a[13] * a5 - a[14] * a4 + a[15] * a3;
		inverse.a[6] = -a[12] * a5 + a[14] * a2 - a[15] * a1;
		inverse.a[10] = +a[12] * a4 - a[13] * a2 + a[15] * a0;
		inverse.a[14] = -a[12] * a3 + a[13] * a1 - a[14] * a0;
		inverse.a[3] = -a[9] * a5 + a[10] * a4 - a[11] * a3;
		inverse.a[7] = +a[8] * a5 - a[10] * a2 + a[11] * a1;
		inverse.a[11] = -a[8] * a4 + a[9] * a2 - a[11] * a0;
		inverse.a[15] = +a[8] * a3 - a[9] * a1 + a[10] * a0;
		float invDet = ((float)1) / det;
		for(int i = 0; i < 16; ++i){
			inverse.a[i] *= invDet;
		}
	}
	return inverse;
}

Mtx44 Mtx44::operator*(const Mtx44 &RHS) const{ //For matrix-matrix multiplication
	Mtx44 ret;
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			ret.a[i * 4 + j] = a[0 * 4 + j] * RHS.a[i * 4 + 0] + a[1 * 4 + j] * RHS.a[i * 4 + 1] + a[2 * 4 + j] * RHS.a[i * 4 + 2] + a[3 * 4 + j] * RHS.a[i * 4 + 3];
		}
	}
	return ret;
}

Mtx44 Mtx44::operator+(const Mtx44 &RHS) const{ //For matrix-matrix addition
	float v[16];
	for(int i = 0; i < 16; ++i){
		v[i] = a[i] + RHS.a[i];
	}
	return Mtx44(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
}

Mtx44& Mtx44::operator=(const Mtx44 &RHS){
	for(int i = 0; i < 16; ++i){
		a[i] = RHS.a[i];
	}
	return *this;
}

Mtx44 Mtx44::operator*(float scalar) const{ //For scalar multiplication
	Mtx44 ret;
	for(int i = 0; i < 16; ++i){
		ret.a[i] = scalar * a[i];
	}
	return ret;
}

void Mtx44::SetToRotation(float degrees, float axisX, float axisY, float axisZ) throw(DivideByZero){ //Set matrix to a rotation matrix about an arbitrary axis
	double mag = sqrt(axisX * axisX + axisY * axisY + axisZ * axisZ);
	if(Math::FAbs((float)mag) < Math::EPSILON){
		throw DivideByZero(); //Thrown if rotation axis is a zero/... vector
	}
	double x = axisX / mag, y = axisY / mag, z = axisZ / mag, c = cos(degrees * Math::PI / 180), s = sin(degrees * Math::PI / 180);
	a[0] = float(x * x * (1.f - c) + c);
	a[1] = float(y * x * (1.f - c) + z * s);
	a[2] = float(x * z * (1.f - c) - y * s);
	a[3] = 0;
	a[4] = float(x * y * (1.f - c) - z * s);
	a[5] = float(y * y * (1.f - c) + c);
	a[6] = float(y * z * (1.f - c) + x * s);
	a[7] = 0;
	a[8] = float(x * z * (1.f - c) + y * s);
	a[9] = float(y * z * (1.f - c) - x * s);
	a[10] = float(z * z * (1.f - c) + c);
	a[11] = 0;
	a[12] = 0;
	a[13] = 0;
	a[14] = 0;
	a[15] = 1;
}

void Mtx44::SetToScale(float sx, float sy, float sz){ //Set...
	SetToIdentity();
	a[0] = sx;
	a[5] = sy;
	a[10] = sz;
}

void Mtx44::SetToTranslation(float tx, float ty, float tz){ //Set...
	SetToIdentity();
	a[12] = tx;
	a[13] = ty;
	a[14] = tz;
}

void Mtx44::SetToFrustum(double left, double right, double bottom, double top, double near, double far){ //Set...
	*this = Mtx44(float(2 * near / (right - left)), 0, 0, 0, 0, float(2 * near / (top - bottom)), 0, 0, float((right + left) / (right - left)),
		float((top + bottom) / (top - bottom)), -float((far + near) / (far - near)), -1, 0, 0, -float(2 * far * near / (far - near)), 0);
}

void Mtx44::SetToLookAt(double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ, double upX, double upY, double upZ){
	Vector3 f(float(centerX - eyeX), float(centerY - eyeY), float(centerZ - eyeZ));
	f.Normalize();
	Vector3 up((float)upX, (float)upY, (float)upZ);
	up.Normalize();
	Vector3 s = f.Cross(up);
	Vector3 u = s.Cross(f);
	Mtx44 mat(s.x, u.x, -f.x, 0, s.y, u.y, -f.y, 0, s.z, u.z, -f.z, 0, 0, 0, 0, 1), tran;
	tran.SetToTranslation(-(float)eyeX, -(float)eyeY, -(float)eyeZ);
	*this = mat * tran;
}

void Mtx44::SetToPerspective(double fovy, double aspectRatio, double zNear, double zFar){ //Set matrix to a perspective matrix
	double f = 1.0 / tan(Math::PI / 180 * fovy / 2);
	*this = Mtx44(float(f / aspectRatio), 0, 0, 0, 0, (float)f, 0, 0, 0, 0, float((zFar + zNear) / (zNear - zFar)), -1, 0, 0, float(2 * zFar * zNear / (zNear - zFar)), 0);
}

void Mtx44::SetToOrtho(double left, double right, double bottom, double top, double nearVal, double farVal){ //Set matrix to an orthogonal matrix (..., param nearVal [View box - front], param farVal [View box - back])
	*this = Mtx44(2 / float(right - left), 0, 0, 0, 0, 2 / float(top - bottom), 0, 0, 0, 0, -2 / float(farVal - nearVal), 0,
		-float((right + left) / (right - left)), -float((top + bottom) / (top - bottom)), -float((farVal + nearVal) / (farVal - nearVal)), 1);
}

Vector3 Mtx44::operator*(const Vector3 &RHS) const{
	float b[4];
	for(int i = 0; i < 4; i++){
		b[i] = a[0 * 4 + i] * RHS.x + a[1 * 4 + i] * RHS.y + a[2 * 4 + i] * RHS.z + a[3 * 4 + i] * 0;
	}
	return Vector3(b[0], b[1], b[2]);
}