#include "SkeletonControler.h"
#include <src/gKit/window.h>

SkeletonControler::SkeletonControler() :CharacterController() {
	iddel.init(smart_path("data/bvh/motionGraph/null.bvh"));
	walk.init(smart_path("data/bvh/motionGraph/marcher.bvh"));
	run.init(smart_path("data/bvh/motionGraph/courir.bvh"));
	kick.init(smart_path("data/bvh/motionGraph/frapper.bvh"));
	dance.init(smart_path("data/bvh/motionGraph/dancer.bvh"));

	Mg.addMotion(walk);
	Mg.addMotion(run);
	Mg.addMotion(kick);
	Mg.addMotion(dance);

	sk.init(iddel);
	pf_iddel = pf_walk = pf_run = pf_kick = pf_dance = 0;
}

Skeleton& SkeletonControler::getSkeleton() {
	return sk;
}

Transform& SkeletonControler::getPos() {
	return m_ch2w;
}


static bool kickPush, dancePush;
void SkeletonControler::update(const float dt) {
	CharacterController::update(dt);

	if (key_event().type == SDL_KEYUP) {
		pf_kick = 0;
		kickPush = false;
		pf_dance = 0;
		pf_kick = 0;
	}

	if (key_state('x') || key_state('X')) {
		m_v = 0;
		sk.setPose(kick, pf_kick);
		pf_kick++;
	}
	else if (key_state('c') || key_state('C')) {

		sk.setPose(dance, pf_dance);
		pf_dance++;
		m_v = 0;

	}
	else {
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
		else if (m_v < m_MAX_V * 0.6 && m_v > m_vMax * 0.9) {
			MotionGraph::GrapheNode node_walk = Mg.getNode(0, pf_walk);
			MotionGraph::GrapheNode node_run = Mg.getNextFrame(node_walk, 1);
			pf_run = node_run.frame;
			sk.setPoseInterpolation(walk, pf_walk, run, pf_run, m_v - m_MAX_V * 0.5);
			pf_walk++;
		}
		else {
			sk.setPose(run, pf_run);
			pf_run++;
		}
	}

}
