#include "Tissus.h"

 Tissus::Tissus(int sizeX, int sizeY, Point pos, float l0, float k, float v) : points(Array2D<Particle>(sizeX, sizeY)) {
	float poidsParicles = poidsTissus / (float)(sizeX * sizeY);

	float l0diag = sqrt(2 * l0 * l0);

	int x, y;
	for (y = 0; y < sizeY; y++) {
		for (x = 0; x < sizeX; x++) {
			Particle p(sizeParticule, poidsParicles, Point(pos.x + x * l0, pos.y - y * l0, pos.z));
			points(x, y) = p;
		}
	}
	points(0, 0).fixParticle();
	points(sizeX - 1, 0).fixParticle();

	Color tisCol = Color(0.5, 0.5, 1);
	Point p;
	for (y = 0; y < sizeY - 1; y++) {
		for (x = 0; x < sizeX - 1; x++) {
			mesh.color(tisCol);
			mesh.vertex(points(x + 1, y).position());

			mesh.color(tisCol);
			mesh.vertex(points(x, y).position());

			mesh.color(tisCol);
			mesh.vertex(points(x, y + 1).position());

			mesh.color(tisCol);
			mesh.vertex(points(x + 1, y).position());

			mesh.color(tisCol);
			mesh.vertex(points(x, y + 1).position());

			mesh.color(tisCol);
			mesh.vertex(points(x + 1, y + 1).position());
		}
	}

	for (y = 0; y < points.sizeY() - 1; y++) {
		for (x = 0; x < points.sizeX() - 1; x++) {
			ressors.push_back(Spring(&points(x, y), &points(x + 1, y), l0, v, k));
			ressors.push_back(Spring(&points(x, y), &points(x, y + 1), l0, v, k));
			ressors.push_back(Spring(&points(x, y), &points(x + 1, y + 1), l0diag, v, k));
			ressors.push_back(Spring(&points(x + 1, y), &points(x, y + 1), l0diag, v, k));
		}
	}
	for (y = 0; y < points.sizeY() - 1; y++) {
		ressors.push_back(Spring(&points(points.sizeX() - 1, y), &points(points.sizeX() - 1, y + 1), l0, v, k));
	}
	for (x = 0; x < points.sizeX() - 1; x++) {
		ressors.push_back(Spring(&points(x, points.sizeY() - 1), &points(x + 1, points.sizeY() - 1), l0, v, k));
	}

}

  void Tissus::updateMesh() {
	 int id = 0, x, y;
	 for (y = 0; y < points.sizeY() - 1; y++) {
		 for (x = 0; x < points.sizeX() - 1; x++) {
			 mesh.vertex(id, points(x + 1, y).position());
			 id++;
			 mesh.vertex(id, points(x, y).position());
			 id++;
			 mesh.vertex(id, points(x, y + 1).position());
			 id++;
			 mesh.vertex(id, points(x + 1, y).position());
			 id++;
			 mesh.vertex(id, points(x, y + 1).position());
			 id++;
			 mesh.vertex(id, points(x + 1, y + 1).position());
			 id++;
		 }
	 }
 }

Mesh& Tissus::getTissus() {return mesh;}

Array2D<Particle>& Tissus::getParticules() { return points; }

std::vector<Spring>& Tissus::getSprings() { return ressors; }
