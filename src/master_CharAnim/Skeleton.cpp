
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
	// Multiplier la matrice l2f avec la matrice l2w (world<-local) du père qui est déjà stocké dans le tableau 
	// Attention il peut ne pas y avoir de père (pour la racine)

}


static Transform interpolationMatrix(Transform& a, Transform& b, float r) {
	Transform res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.m[i][j] = a.m[i][j] * (1 - r) + b.m[i][j] * (r);
		}
	}
	return res;
}

void Skeleton::setPoseInterpolation(const BVH& bvhSrc, int frameNbSrc, const BVH& bvhDst, int frameNbDst, float t) {

	int frame_s = std::abs(frameNbSrc) % bvhSrc.getNumberOfFrame();
	int frame_d = std::abs(frameNbDst) % bvhDst.getNumberOfFrame();

	for (int i = 0; i < bvhSrc.getNumberOfJoint(); i++) {
		BVHJoint bvhj_s = bvhSrc.getJoint(i);
		BVHJoint bvhj_d = bvhDst.getJoint(i);

		SkeletonJoint joint;
		joint.m_parentId = bvhj_s.getParentId();

		float x_s, y_s, z_s, x_d, y_d, z_d;
		bvhj_s.getOffset(x_s, y_s, z_s);
		bvhj_d.getOffset(x_d, y_d, z_d);
		Transform l2f = interpolationMatrix(Translation(x_s, y_s, z_s), Translation(x_d, y_d, z_d), t);

		float xt = 0, yt = 0, zt = 0;
		for (int j = 0; j < bvhj_s.getNumberOfChannel(); j++) {
			BVHChannel chan_s = bvhj_s.getChannel(j);
			BVHChannel chan_d = bvhj_d.getChannel(j);

			switch (chan_s.getAxis()) {
			case AXIS_X:
				(chan_s.isRotation()) ?
					l2f = l2f(interpolationMatrix(RotationX(chan_s.getData(frame_s)), RotationX(chan_d.getData(frame_d)), t)) :
					l2f = l2f(interpolationMatrix(Translation(chan_s.getData(frame_s), 0, 0), Translation(chan_d.getData(frame_d), 0, 0), t));

				break;
			case AXIS_Y:
				(chan_s.isRotation()) ?
					l2f = l2f(interpolationMatrix(RotationY(chan_s.getData(frame_s)), RotationY(chan_d.getData(frame_d)), t)) :
					l2f = l2f(interpolationMatrix(Translation(0, chan_s.getData(frame_s), 0), Translation(0, chan_d.getData(frame_d), 0), t));

				break;
			case AXIS_Z:
				(chan_s.isRotation()) ?
					l2f = l2f(interpolationMatrix(RotationZ(chan_s.getData(frame_s)), RotationZ(chan_d.getData(frame_d)), t)) :
					l2f = l2f(interpolationMatrix(Translation(0, 0, chan_s.getData(frame_s)), Translation(0, 0, chan_d.getData(frame_d)), t));

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
	// Multiplier la matrice l2f avec la matrice l2w (world<-local) du père qui est déjà stocké dans le tableau 
	// Attention il peut ne pas y avoir de père (pour la racine)


}

float distance(const Skeleton::CASkeleton& a, const Skeleton::CASkeleton& b)
{
	a.sk.setPose(a.bvh, a.frame);
	b.sk.setPose(b.bvh, b.frame);

	float diff = 0;

	for (int i = 0; i < a.sk.numberOfJoint(); i++) {
		diff = std::max(diff, distance2(a.sk.getJointPosition(i), b.sk.getJointPosition(i)));
	}

	return diff;
}
