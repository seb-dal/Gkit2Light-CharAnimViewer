/****************************************************************************
Copyright (C) 2010-2011 Alexandre Meyer

This file is part of  library.


Sime is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Sime is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Sime.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef _TRANSFORMQ_H
#define _TRANSFORMQ_H

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <iostream>
#include <vec.h>


/*! \brief
 */
class TransformQ
{
public:
	TransformQ() : T(0, 0, 0) {}
	TransformQ(const Vector& _T) : T(_T) {}

	//! constructor from axis and angle (degree) of rotation
	TransformQ(const Vector& axis_rotation, const float angle, const Vector& translation = Vector(0, 0, 0)) : Q(axis_rotation, angle), T(translation) {}

	TransformQ(const Quaternion& _Q, const Vector& _T = Vector(0, 0, 0)) : Q(_Q), T(_T) {}


	/*! \brief Tr x Tr */
	friend inline TransformQ operator*(const TransformQ& a, const TransformQ& b) {
		return TransformQ(a.Q * b.Q, a.T * b.T);
	}

	friend inline Vector 		operator*(const TransformQ& a, const Vector& v)
	{
		float x = v.x;
		float y = v.y;
		float z = v.z;
		float m[3][3];
		a.Q.getMatrix33(m);

		float xt = m[0][0] * x + m[0][1] * y + m[0][2] * z;                  // dot(vec4(m[0]), vec4(v, 0))
		float yt = m[1][0] * x + m[1][1] * y + m[1][2] * z;                  // dot(vec4(m[1]), vec4(v, 0))
		float zt = m[2][0] * x + m[2][1] * y + m[2][2] * z;                  // dot(vec4(m[2]), vec4(v, 0))
		// dot(vec4(m[3]), vec4(v, 0)) == dot(vec4(0, 0, 0, 1), vec4(v, 0)) == 0 par definition

		return Vector(xt, yt, zt) * a.T;
	}

	friend inline Point 		operator*(const TransformQ& a, const Point& v)
	{
		float x = v.x;
		float y = v.y;
		float z = v.z;
		float m[3][3];
		a.Q.getMatrix33(m);

		float xt = m[0][0] * x + m[0][1] * y + m[0][2] * z;                  // dot(vec4(m[0]), vec4(v, 0))
		float yt = m[1][0] * x + m[1][1] * y + m[1][2] * z;                  // dot(vec4(m[1]), vec4(v, 0))
		float zt = m[2][0] * x + m[2][1] * y + m[2][2] * z;                  // dot(vec4(m[2]), vec4(v, 0))
		// dot(vec4(m[3]), vec4(v, 0)) == dot(vec4(0, 0, 0, 1), vec4(v, 0)) == 0 par definition

		return Point(Vector(xt, yt, zt) * a.T);
	}


	inline static TransformQ slerp(const TransformQ& a, const TransformQ& b, float t)
	{
		Quaternion Qr;
		Qr.slerp(a.Q, b.Q, t, true);
		return TransformQ(Qr, a.T * (1 - t) + b.T * t);
	}

	void setIdentity()
	{
		T = Vector(0, 0, 0);
		Q.setAxisAngle(Vector(1, 0, 0), 0.f);
	}

	TransformQ getInverse() const
	{
		Quaternion NQ = Q.inverse();
		return TransformQ(NQ, NQ * Vector(-T.x, -T.y, -T.z));
	}

	void inverse()
	{
		*this = getInverse();
	}

	void setRotation(const Quaternion& _Q) { Q = _Q; }
	void setTranslation(const Vector& _T) { T = _T; }
	Quaternion& getRotation() { return Q; }
	Vector& getTranslation() { return T; }
	const Quaternion& getRotation() const { return Q; }
	const Vector& getTranslation() const { return T; }

	template<typename MAT>
	void getMatrix44(MAT& m) const
	{
		Q.getMatrix44(m);
		m[0][3] = T.x;
		m[1][3] = T.y;
		m[2][3] = T.z;
	}

	void getMatrix16(float m[16]) const
	{
		Q.getMatrix16(m);
		m[3] = T.x;
		m[7] = T.y;
		m[11] = T.z;
	}

	friend inline std::ostream& operator<<(std::ostream& o, const TransformQ& a)
	{
		o << "Q=" << a.Q << ", T=" << a.T;
		return o;
	}

protected:
	Quaternion Q;
	Vector T;
};



#endif

