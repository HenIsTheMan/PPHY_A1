#pragma once
#include <stack>
#include "Mtx44.h"

class MS final{
	std::stack<Mtx44> ms;
public:
	MS();
	const Mtx44& Top() const;
	void PopMatrix(), PushMatrix(), Clear(), LoadIdentity(), LoadMatrix(const Mtx44&), MultMatrix(const Mtx44&);
	void Rotate(float, float, float, float), Scale(float, float, float), Translate(float, float, float);
	void Frustum(double, double, double, double, double, double);
	void LookAt(double, double, double, double, double, double, double, double, double);
};