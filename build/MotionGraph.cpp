#include "MotionGraph.h"
#include "Skeleton.h"

MotionGraph::BVH_ID MotionGraph::addMotion(chara::BVH& motion) {
	l_BVH_ID[motion] = m_BVH.size();
	m_BVH.push_back(motion);

	Skeleton sk1, sk2;
	for (chara::BVH& m : m_BVH) {
		for (int nf = 0; nf < motion.getNumberOfFrame(); nf++) {
			for (int f = 0; f < m.getNumberOfFrame(); f++) {

			}
		}
	}


	return l_BVH_ID[motion];
}

MotionGraph::BVH_ID MotionGraph::getMotion_ID(chara::BVH& motion) {
	return l_BVH_ID[motion];
}
