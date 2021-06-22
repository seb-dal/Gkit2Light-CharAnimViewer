#pragma once

#include "Array2D.h"
#include <src/gKit/vec.h>
#include <src/master_CharAnim/Particle.h>
#include <src/gKit/mesh.h>
#include <src/master_CharAnim/Spring.h>


class Tissus {
public:
	float sizeParticule = 20;
	float poidsTissus = 0.5;
	Tissus(int sizeX, int sizeY, Point pos, float l0, float k, float v);;

	void updateMesh();

	Mesh& getTissus();

	Array2D<Particle>& getParticules();
	std::vector<Spring>& getSprings();

private:
	Array2D<Particle> points;
	std::vector<Spring> ressors;

	Mesh mesh;
};

