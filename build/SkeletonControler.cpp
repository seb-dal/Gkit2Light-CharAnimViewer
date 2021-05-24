#include "SkeletonControler.h"
#include <src/gKit/window.h>

SkeletonControler::SkeletonControler() :CharacterController() {
	iddel.init(smart_path("data/bvh/motionGraph/null.bvh"));
	walk.init(smart_path("data/bvh/motionGraph/marcher.bvh"));
	run.init(smart_path("data/bvh/motionGraph/courir.bvh"));
	kick.init(smart_path("data/bvh/motionGraph/frapper.bvh"));
	dance.init(smart_path("data/bvh/motionGraph/dancer.bvh"));
	sk.init(iddel);
	pf_iddel = pf_walk = pf_run = pf_kick = pf_dance = 0;
}

Skeleton& SkeletonControler::getSkeleton() {
	return sk;
}

Transform& SkeletonControler::getPos() {
	return m_ch2w;
}

void SkeletonControler::update(const float dt) {
	CharacterController::update(dt);



	if (m_v == 0) {
		sk.setPose(iddel, pf_iddel);
		pf_iddel++;

	}
	else if (m_v < m_MAX_V * 0.1) {
		sk.setPoseInterpolation(iddel, pf_iddel, walk, pf_walk, m_v);
		pf_iddel++;
		pf_walk++;
	}
	else if (m_v < m_MAX_V * 0.5) {
		sk.setPose(walk, pf_walk);
		pf_walk++;
	}
	else if (m_v < m_MAX_V * 0.6) {
		sk.setPoseInterpolation(walk, pf_walk, run, pf_run, m_v - m_MAX_V * 0.5);
		pf_walk++;
		pf_run++;
	}
	else {
		sk.setPose(run, pf_run);
		pf_run++;
	}

}
