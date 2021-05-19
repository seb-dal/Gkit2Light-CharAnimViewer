
#include "Skeleton.h"

using namespace chara;

void Skeleton::init(const BVH& bvh)
{
	m_joints.resize(bvh.getNumberOfJoint());

	for (int i = 0; i < bvh.getNumberOfJoint(); i++) {
		BVHJoint bvhj = bvh.getJoint(i);

		SkeletonJoint joint;
		joint.m_parentId = bvhj.getParentId();

		float x, y, z;
		bvhj.getOffset(x, y, z);
		joint.m_l2w = Translation(x, y, z);

		if (joint.m_parentId != -1) {
			joint.m_l2w = joint.m_l2w(m_joints.at(joint.m_parentId).m_l2w);
		}

		m_joints.at(i) = joint;
	}
}


Point Skeleton::getJointPosition(int i) const
{
	return m_joints.at(i).m_l2w(Point(0, 0, 0));
}


int Skeleton::getParentId(const int i) const
{
	return m_joints.at(i).m_parentId;
}


void Skeleton::setPose(const BVH& bvh, int frameNumber)
{
	int frame = std::abs(frameNumber) % bvh.getNumberOfFrame();

	for (int i = 0; i < bvh.getNumberOfJoint(); i++) {
		BVHJoint bvhj = bvh.getJoint(i);
		SkeletonJoint joint;
		joint.m_parentId = bvhj.getParentId();

		float x, y, z;
		bvhj.getOffset(x, y, z);
		Transform l2f = Translation(x, y, z);

		float xt = 0, yt = 0, zt = 0;
		for (int j = 0; j < bvhj.getNumberOfChannel(); j++) {
			BVHChannel chan = bvhj.getChannel(j);
			switch (chan.getAxis()) {
			case AXIS_X:
				(chan.isRotation()) ?
					l2f = l2f(RotationX(chan.getData(frame))) :
					l2f = l2f(Translation(chan.getData(frame), 0, 0));

				break;
			case AXIS_Y:
				(chan.isRotation()) ?
					l2f = l2f(RotationY(chan.getData(frame))) :
					l2f = l2f(Translation(0, chan.getData(frame), 0));

				break;
			case AXIS_Z:
				(chan.isRotation()) ?
					l2f = l2f(RotationZ(chan.getData(frame))) :
					l2f = l2f(Translation(0, 0, chan.getData(frame)));

				break;
				/*case AXIS_W:
					break;*/
			}
		}

		joint.m_l2w = (joint.m_parentId != -1) ?
			m_joints.at(joint.m_parentId).m_l2w(l2f) :
			l2f;

		m_joints.at(i) = joint;
	}

	// TODO
	// Parcourir toutes les articulations (SkeletonJoint ou BVHJoint) 
	//     Declarer la matrice l2f (pere<-local)
	//     Init avec la translation offset
	//     Parcourir tous les channels
	//          Accumuler dans la matrice l2f les translations et rotation en fonction du type de Channel
	// Multiplier la matrice l2f avec la matrice l2w (world<-local) du p�re qui est d�j� stock� dans le tableau 
	// Attention il peut ne pas y avoir de p�re (pour la racine)

}
