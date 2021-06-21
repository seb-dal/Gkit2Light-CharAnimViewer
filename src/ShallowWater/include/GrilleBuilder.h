#pragma once
#include <src/gKit/mesh.h>

class GrilleBuilder {
public:
	static Mesh* new_build(const int mx, const int Mx, const int my, const int My, const float spaceBetween) {
		Mesh* m = new Mesh(GL_TRIANGLES);
		build(*m, mx, Mx, my, My, spaceBetween);
		return m;
	}

	static void build(Mesh& obj, const int mx, const int Mx, const int my, const int My, const float spaceBetween) {
		int x, y;
		float xx, xxp, zz, zzp;
		for (x = mx; x < Mx - 1; x++) {
			for (y = my; y < My - 1; y++) {
				xx = x * spaceBetween;
				xxp = xx + spaceBetween;
				zz = y * spaceBetween;
				zzp = zz + spaceBetween;

				obj.color(White());
				obj.vertex(xxp, 0, zz);

				obj.color(White());
				obj.vertex(xx, 0, zz);

				obj.color(White());
				obj.vertex(xx, 0, zzp);


				obj.color(White());
				obj.vertex(xxp, 0, zz);

				obj.color(White());
				obj.vertex(xx, 0, zzp);

				obj.color(White());
				obj.vertex(xxp, 0, zzp);
			}
		}
	}

};
