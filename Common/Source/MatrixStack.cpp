#include "MatrixStack.h"

MS::MS(){
	Mtx44 mat;
	mat.SetToIdentity();
	ms.push(mat);
}

const Mtx44& MS::Top() const{ //Return a copy of the top matrix of the matrix stack
	return ms.top();
}

void MS::PopMatrix(){ //Pop the top matrix...
	ms.pop();
}

void MS::PushMatrix(){ //Make a copy of the top matrix... and push it on top
	ms.push(ms.top());
}

void MS::Clear(){ //Clear the matrix stack
	while(ms.size() > 1){
		ms.pop();
	}
}

void MS::LoadIdentity(){ //Replace the top matrix... with an identity matrix
	Mtx44 mat;
	mat.SetToIdentity();
	ms.top() = mat;
}

void MS::LoadMatrix(const Mtx44 &matrix){ //Replace the top matrix... with a new matrix
	ms.top() = matrix;
}

void MS::MultMatrix(const Mtx44 &matrix){ //Multiply the top matrix... with a new matrix
	ms.top() = ms.top() * matrix; //*= operator not overloaded so cannot be used here
}

void MS::Rotate(float degrees, float axisX, float axisY, float axisZ){ //Multiply the top matrix... with a rotation matrix (...clockwise, param axisX [X-componenent of the rotation axis], ...)
	Mtx44 mat;
	mat.SetToRotation(degrees, axisX, axisY, axisZ);
	ms.top() = ms.top() * mat;
}

void MS::Scale(float scaleX, float scaleY, float scaleZ){ //Multiply...
	Mtx44 mat;
	mat.SetToScale(scaleX, scaleY, scaleZ);
	ms.top() = ms.top() * mat;
}

void MS::Translate(float translateX, float translateY, float translateZ){ //Multiply... (param translateX [offset along x-axis], ...)
	Mtx44 mat;
	mat.SetToTranslation(translateX, translateY, translateZ);
	ms.top() = ms.top() * mat;
}

void MS::Frustum(double left, double right, double bottom, double top, double near, double far){ //Setup frustum matrix and push into matrix stack (..., param near [Frustum - front], param far [Frustum - back])
	Mtx44 mat;
	mat.SetToFrustum(left, right, bottom, top, near, far);
	ms.top() = ms.top() * mat;
}

void MS::LookAt(double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ, double upX, double upY, double upZ){ //Setup lookat matrix and push...
	Mtx44 mat;
	mat.SetToLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	ms.top() = ms.top() * mat;
}