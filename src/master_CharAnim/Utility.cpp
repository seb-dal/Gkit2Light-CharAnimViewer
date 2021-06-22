#include "Utility.h"

bool Utility::between(double min, double max, double value) {
	return min <= value && value <= max;
}

bool Utility::betweenP(double valueCheck, double value, double percentage) {
	return between(value * (1 - percentage), value * (1 + percentage), valueCheck);
}

Vector Utility::Extract_col(const Transform& t, int col) {
	return Vector(t.m[0][col], t.m[1][col], t.m[2][col]);
}

Vector Utility::Extract_translate(const Transform& t) {
	return Extract_col(t, 3);
}

Vector Utility::Extract_Scale(const Transform& t) {
	return Vector(
		length(Extract_col(t, 0)),
		length(Extract_col(t, 1)),
		length(Extract_col(t, 2))
	);
}

Transform Utility::Extract_Rotation(const Transform& t) {
	Vector scale = Extract_Scale(t);
	return Transform(
		Extract_col(t, 0) / scale(0),
		Extract_col(t, 1) / scale(1),
		Extract_col(t, 2) / scale(2),
		Vector(0, 0, 0)
	);
}

void Utility::Extract_All(const Transform& t, Vector& scale, Vector& translation, Transform& rot) {
	Vector c0 = Extract_col(t, 0),
		c1 = Extract_col(t, 1),
		c2 = Extract_col(t, 2);
	translation = Extract_col(t, 3);
	scale = Vector(length(c0), length(c1), length(c2));

	rot = Transform(
		c0 / scale(0),
		c1 / scale(1),
		c2 / scale(2),
		Vector(0, 0, 0)
	);
}

Transform Utility::slerp(Transform& a, Transform& b, float r) {
	int i, j;
	Transform res;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			res.m[i][j] = a.m[i][j] * (1 - r) + b.m[i][j] * (r);
		}
	}
	return res;
}

Transform Utility::slerpPos(Transform& a, Transform& b, float r) {
	int i, j;
	Vector sc1, sc2, scr, tra1, tra2, trar;
	Transform rot1, rot2, rotr, res;
	Extract_All(a, sc1, tra1, rot1);
	Extract_All(b, sc2, tra2, rot2);

	scr = sc1 * (1 - r) + sc2 * (r);
	trar = tra1 * (1 - r) + tra2 * (r);
	rotr = slerp(rot1, rot2, r);

	return rotr(Scale(scr.x, scr.y, scr.z))(Translation(trar));
}
