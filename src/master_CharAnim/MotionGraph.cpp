#include "MotionGraph.h"
#include "Skeleton.h"


MotionGraph::BVH_ID MotionGraph::addMotion(chara::BVH& motion) {
	int i = m_GrapheNode.size();


	int nf, // n° of the frame of motion
		bvhid, // id of a BVH
		iii, // min n° motion in a BVH to another
		ii,
		iiii,
		f; // n° of the frame of others motion

	float
		d, // distance between motion min
		dist_ref,
		dist;

	std::string key;
	GrapheNode gn;
	Skeleton sk1, sk2;
	sk1.init(motion);
	sk2.init(motion);
	for (nf = 0; nf < motion.getNumberOfFrame(); nf++) {
		gn = GrapheNode();
		gn.frame = nf;
		gn.id_bvh = m_BVH.size();
		gn.ids_next.push_back(nf + 1 % motion.getNumberOfFrame());
		m_GrapheNode.push_back(gn);
	}


	for (nf = 0; nf < motion.getNumberOfFrame(); nf++) {
		ii = 0;
		for (bvhid = 0; bvhid < m_BVH.size(); bvhid++) {
			chara::BVH bvh = m_BVH[bvhid];
			iii = ii;
			d = 1.9e38;

			for (f = bvh.getNumberOfFrame() - 1; f > -1; f--) {
				dist = Skeleton::distance(
					Skeleton::CASkeleton(sk1, motion, nf),
					Skeleton::CASkeleton(sk2, bvh, f)
				);

				if (dist < d) {
					d = dist;
					iii = ii;
				}
				ii++;
			}

			m_GrapheNode[i + nf].ids_next.push_back(iii);
		}
	}

	iiii = 0;
	for (bvhid = 0; bvhid < m_BVH.size(); bvhid++) {
		chara::BVH bvh = m_BVH[bvhid];

		for (f = bvh.getNumberOfFrame() - 1; f > -1; f--) {
			d = 1.9e38;
			for (nf = 0; nf < motion.getNumberOfFrame(); nf++) {

				dist = Skeleton::distance(
					Skeleton::CASkeleton(sk1, motion, nf),
					Skeleton::CASkeleton(sk2, bvh, f)
				);

				if (dist < d) {
					d = dist;
					iii = nf;
				}

			}
			m_GrapheNode[iiii].ids_next.push_back(i + nf);
			iiii++;
		}
	}


	m_BVH.push_back(motion);
	m_GrapheNodeGet.clear();

	for (ii = 0; ii < m_GrapheNode.size(); ii++) {
		gn = m_GrapheNode.at(ii);
		key = tokey(gn.id_bvh, gn.frame);

		m_GrapheNodeGet[key] = gn;
	}





	return m_BVH.size() - 1;
}



MotionGraph::GrapheNode& MotionGraph::getNextFrame(GrapheNode& actu, BVH_ID objectif) {
	if (actu.id_bvh != objectif) {
		for (int gnn : actu.ids_next) {
			if (m_GrapheNode[gnn].id_bvh == objectif) {
				return m_GrapheNode[gnn];
			}
		}
	}
	for (int gnn : actu.ids_next) {
		if (m_GrapheNode[gnn].id_bvh == actu.id_bvh) {
			return m_GrapheNode[gnn];
		}
	}


}
