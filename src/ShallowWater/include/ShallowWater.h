#pragma once
#include <src/ShallowWater/include/Array2D.h>
#include <src/gKit/mesh.h>

class ShallowWater
{
public:
	ShallowWater();
	void init(const int DIMX, const int DIMY);
	void drawSol(Mesh& sol, const int mx, const int Mx, const int my, const int My, const float spaceBetween);
	void draw(Mesh& m, const int mx, const int Mx, const int my, const int My, const float spaceBetween);
	void computeOneStep();

	static void advect(const Array2D& s, Array2D& dest, const Array2D& deplX, const Array2D& deplY, float dt, float dxy);
	static void advect3(const Array2D& s1, Array2D& dest1, const Array2D& s2, Array2D& dest2, const Array2D& s3, Array2D& dest3, const Array2D& deplX, const Array2D& deplY, float dt, float dxy);
protected:

	float hmin(int i, int j);

	float hmax(int i, int j);

	float nmax(int i, int j);

	void update_rf();


	void update_height();
	void update_velocity();
	void update_h();
protected:
	float dt, dxy;

	Array2D m_g;     // height of the ground (0 if flat)
	Array2D m_h;     // height of the water : the thing to compute and to draw
	Array2D m_n;     // m_n = m_h - m_g : amount of water above the ground
	Array2D m_vX;    // velocity along X   
	Array2D m_vY;    // velocity along Y

	Array2D m_r;
	Array2D_bool m_f; // flag   0:dry   1:fluid

	const float epsilon_h = 0.01;
};
