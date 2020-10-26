#include "Utility.h"

Position operator*(const Mtx44& LHS, const Position& RHS){
	float b[4];
	for(int i = 0; i < 4; i++){
		b[i] = LHS.a[0 * 4 + i] * RHS.x + LHS.a[1 * 4 + i] * RHS.y + LHS.a[2 * 4 + i] * RHS.z + LHS.a[3 * 4 + i] * 1;
	}
	return Position(b[0], b[1], b[2]);
}