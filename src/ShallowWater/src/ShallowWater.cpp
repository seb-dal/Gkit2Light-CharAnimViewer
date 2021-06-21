#include "../include/ShallowWater.h"
#include <src/gKit/window.h>
#include <src/ShallowWater/include/PerlinNoise.hpp>

//#include <omp.h>

static int clamps(int value, int minv, int maxv) {
	return std::min(std::max(value, minv), maxv);
}
static int reflect(int value, int minv, int maxv) {
	if (minv <= value && value <= maxv) { return value; }
	else if (value < minv) { return reflect(2 * minv - value, minv, maxv); }
	else { return  reflect(2 * maxv - value, minv, maxv); }
}

ShallowWater::ShallowWater() {}

void ShallowWater::init(const int DIMX, const int DIMY) {
	dxy = 10;
	dt = 0.1;

	m_g.init(DIMX, DIMY);
	m_h.init(DIMX, DIMY);
	m_n.init(DIMX, DIMY);
	m_vX.init(DIMX, DIMY);
	m_vY.init(DIMX, DIMY);

	m_r.init(DIMX, DIMY);
	m_f.init(DIMX, DIMY);

	siv::PerlinNoise noise2d;
	int i, j;
	for (i = 0; i < DIMX; i++) {
		for (j = 0; j < DIMY; j++) {
			m_g(i, j) = noise2d.normalizedOctaveNoise2D_0_1(0.1 + i * 0.01, 0.1 + j * 0.01, 3) * 50 - 12.5;
			m_n(i, j) = std::max(17.5f - m_g(i, j), 0.f);

			m_h(i, j) = m_g(i, j) + m_n(i, j);
		}
	}

	//update_h();
}

void ShallowWater::drawSol(Mesh& m, const int mx, const int Mx, const int my, const int My, const float spaceBetween) {
	Color c = Color(0.9, 0.85, 0);
	int i, max = m.vertex_count();
	int id = 0;
	int mmx, mmxp, mmy, mmyp;
	float xx, xxp, zz, zzp;
	for (int x = mx; x < Mx - 1; x++) {
		for (int y = my; y < My - 1; y++) {
			mmx = x - mx;
			mmxp = mmx + 1;
			mmy = y - my;
			mmyp = mmy + 1;

			xx = x * spaceBetween;
			xxp = xx + spaceBetween;
			zz = y * spaceBetween;
			zzp = zz + spaceBetween;

			m.color(id, c);
			m.vertex(id, xxp, m_g(mmxp, mmy), zz);
			id++;
			m.color(id, c);
			m.vertex(id, xx, m_g(mmx, mmy), zz);
			id++;
			m.color(id, c);
			m.vertex(id, xx, m_g(mmx, mmyp), zzp);


			id++;
			m.color(id, c);
			m.vertex(id, xxp, m_g(mmxp, mmy), zz);
			id++;
			m.color(id, c);
			m.vertex(id, xx, m_g(mmx, mmyp), zzp);
			id++;
			m.color(id, c);
			m.vertex(id, xxp, m_g(mmxp, mmyp), zzp);

			id++;
		}
	}

}

void ShallowWater::draw(Mesh& m, const int mx, const int Mx, const int my, const int My, const float spaceBetween) {
	//update_rf();

	int id = 0;
	Color a, b;
	int mmx, mmxp, mmy, mmyp;
	float xx, xxp, zz, zzp;

	int x, y;
	for (x = mx; x < Mx - 1; x++) {
		for (y = my; y < My - 1; y++) {
			mmx = x - mx;
			mmxp = mmx + 1;
			mmy = y - my;
			mmyp = mmy + 1;

			xx = x * spaceBetween;
			xxp = xx + spaceBetween;
			zz = y * spaceBetween;
			zzp = zz + spaceBetween;

			if (m_n(mmxp, mmy) > epsilon_h || m_n(mmx, mmy) > epsilon_h || m_n(mmx, mmyp) > epsilon_h) { a = Blue(); }
			else { a = Color(0.9, 0.85, 0); }

			if (m_n(mmxp, mmy) > epsilon_h || m_n(mmx, mmyp) > epsilon_h || m_n(mmxp, mmyp) > epsilon_h) { b = Blue(); }
			else { b = Color(0.9, 0.85, 0); }

			m.color(id, a);
			m.vertex(id, xxp, m_h(mmxp, mmy), zz);

			id++;
			m.color(id, a);
			m.vertex(id, xx, m_h(mmx, mmy), zz);

			id++;
			m.color(id, a);
			m.vertex(id, xx, m_h(mmx, mmyp), zzp);


			id++;
			m.color(id, b);
			m.vertex(id, xxp, m_h(mmxp, mmy), zz);

			id++;
			m.color(id, b);
			m.vertex(id, xx, m_h(mmx, mmyp), zzp);

			id++;
			m.color(id, b);
			m.vertex(id, xxp, m_h(mmxp, mmyp), zzp);

			id++;
		}
	}

}



void ShallowWater::computeOneStep() {
	if (key_state(SDLK_SPACE)) {
		m_n(m_n.dimX() / 2, m_n.dimY() / 2) += 5;
	}



	Array2D n_ = m_n;
	Array2D vx_ = m_vX;
	Array2D vy_ = m_vY;

	advect3(n_, m_n, vx_, m_vX, vy_, m_vY, vx_, vy_, dt, dxy);
	/*advect(n_, m_n, vx_, vy_, dt, dxy);
	advect(vx_, m_vX, vx_, vy_, dt, dxy);
	advect(vy_, m_vY, vx_, vy_, dt, dxy);*/



	update_height();

	update_h();

	update_velocity();

}

void ShallowWater::advect(const Array2D& src, Array2D& dest, const Array2D& deplX, const Array2D& deplY, float dt, float dxy) {
	//assert(&s == &deplX);
	//assert(&s == &deplY);


	assert(src.dimX() == deplX.dimX() && src.dimY() == deplX.dimY());
	assert(src.dimX() == deplY.dimX() && src.dimY() == deplY.dimY());

	int i, j;
	float x, y;

	for (i = 0; i < src.dimX(); ++i) {
		for (j = 0; j < src.dimY(); ++j) {
			x = (float)i - dt * deplX(i, j);
			y = (float)j - dt * deplY(i, j);
			dest(i, j) = src.interpolate(x, y);
		}
	}

}

void ShallowWater::advect3(const Array2D& s1, Array2D& dest1, const Array2D& s2, Array2D& dest2, const Array2D& s3, Array2D& dest3, const Array2D& deplX, const Array2D& deplY, float dt, float dxy) {
	int i, j;
	float x, y;

	for (i = 1; i < s1.dimX() - 1; ++i) {
		for (j = 1; j < s1.dimY() - 1; ++j) {
			x = (float)i - dt * deplX(i, j);
			y = (float)j - dt * deplY(i, j);
			dest1(i, j) = s1.interpolate(x, y);
			dest2(i, j) = s2.interpolate(x, y);
			dest3(i, j) = s3.interpolate(x, y);
		}
	}
}





float ShallowWater::hmin(int i, int j) {
	float min_h = 1e12;
	int x, y;
	for (x = std::max(0, i - 1); x < std::min(m_n.dimX(), i + 1); x++) {
		for (y = std::max(0, j - 1); y < std::min(m_n.dimX(), j + 1); y++) {
			min_h = std::min(min_h, m_h(x, y));
		}
	}

	return (m_h(i, j) + min_h) / 2.f;
}
float ShallowWater::hmax(int i, int j) {
	float max_h = -1e12;
	int x, y;
	for (x = std::max(0, i - 1); x < std::min(m_n.dimX(), i + 1); x++) {
		for (y = std::max(0, j - 1); y < std::min(m_n.dimX(), j + 1); y++) {
			max_h = std::max(max_h, m_h(x, y));
		}
	}

	return (m_h(i, j) + max_h) / 2.f + epsilon_h;
}
float ShallowWater::nmax(int i, int j) {
	float max_n = -1e12;
	int x, y;
	for (x = std::max(0, i - 1); x < std::min(m_n.dimX(), i + 1); x++) {
		for (y = std::max(0, j - 1); y < std::min(m_n.dimX(), j + 1); y++) {
			max_n = std::max(max_n, m_n(x, y));
		}
	}

	return (m_n(i, j) + max_n) / 2.f;
}





void ShallowWater::update_rf() {
	int i, j;

	for (i = 0; i < m_n.dimX(); ++i) {
		for (j = 0; j < m_n.dimY(); ++j) {
			if (m_h(i, j) <= hmin(i, j) && nmax(i, j) < epsilon_h) {
				m_f(i, j, false);
				m_r(i, j) = 0;
			}
			else if (m_h(i, j) > hmax(i, j)) {
				m_f(i, j, true);
				m_r(i, j) = 1;
			}
			else {
				m_f(i, j, true);
				m_r(i, j) = (m_h(i, j) - hmin(i, j)) / (hmax(i, j) - hmin(i, j));
			}
		}
	}
}






void ShallowWater::update_height() {
	//Array2D n_ = m_n;

	int i, j;
	float divX, divY;

	for (i = 1; i < m_n.dimX() - 1; ++i) {
		for (j = 1; j < m_n.dimY() - 1; ++j) {
			divX = (m_vX(i + 1, j) - m_vX(i - 1, j)) / (dxy);
			divY = (m_vY(i, j + 1) - m_vY(i, j - 1)) / (dxy);

			m_n(i, j) -= m_h(i, j) * (divX + divY) * dt;
		}
	}
}


static const float g = 9.81;
void ShallowWater::update_velocity() {

	//Array2D vx_ = m_vX;
	//Array2D vy_ = m_vY;

	int i, j;
	float gradX, gradY;

	for (i = 1; i < m_n.dimX() - 1; ++i) {
		for (j = 0; j < m_n.dimY(); ++j) {
			gradX = (m_h(i - 1, j) - m_h(i + 1, j)) / (dxy);
			m_vX(i, j) += (g * gradX) * dt;

			if (m_n(i, j) < 0)
				m_vX(i, j) = 0;
		}
	}
	for (i = 0; i < m_n.dimX(); ++i) {
		for (j = 1; j < m_n.dimY() - 1; ++j) {
			gradY = (m_h(i, j - 1) - m_h(i, j + 1)) / (dxy);

			m_vY(i, j) += (g * gradY) * dt;

			if (m_n(i, j) < 0)
				m_vY(i, j) = 0;
		}
	}

	//Boundary
	for (i = 0; i < m_n.dimX(); ++i) {
		m_vX(i, 0) = 0;
		m_vY(i, 0) = 0;
		m_vX(i, m_n.dimY() - 1) = 0;
		m_vY(i, m_n.dimY() - 1) = 0;
	}
	for (j = 0; j < m_n.dimY(); ++j) {
		m_vX(0, j) = 0;
		m_vY(0, j) = 0;
		m_vX(m_n.dimX() - 1, j) = 0;
		m_vY(m_n.dimX() - 1, j) = 0;
	}
}

void ShallowWater::update_h() {

	int i, j;

	for (i = 0; i < m_n.dimX(); ++i) {
		for (j = 0; j < m_n.dimY(); ++j) {
			m_h(i, j) = m_g(i, j) + m_n(i, j);
			//m_n(i, j) = m_h(i, j) - m_g(i, j);
		}
	}
}


