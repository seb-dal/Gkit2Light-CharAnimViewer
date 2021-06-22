#pragma once
#include <src/gKit/mat.h>
class Utility {
public:

	static bool between(double min, double max, double value);

	static bool betweenP(double valueCheck, double value, double percentage);


	static Vector Extract_col(const Transform& t, int col);

	static Vector Extract_translate(const Transform& t);


	static Vector Extract_Scale(const Transform& t);

	static Transform Extract_Rotation(const Transform& t);
	static void Extract_All(const Transform& t, Vector& scale, Vector& translation, Transform& rot);


	static Transform slerp(Transform& a, Transform& b, float r);

	static Transform slerpPos(Transform& a, Transform& b, float r);


	/*static Transform slerpRot(Quaternion& a, Quaternion& b, float r) {
		Quaternion slerpr = Quaternion::slerp(a, b, r, true);
		Transform res = Transform(
			slerpr[0], slerpr[1], slerpr[2], slerpr[3],
			slerpr[4], slerpr[5], slerpr[6], slerpr[7],
			slerpr[8], slerpr[9], slerpr[10], slerpr[11],
			slerpr[12], slerpr[13], slerpr[14], slerpr[15]
		);

		return res;
	}*/

};

