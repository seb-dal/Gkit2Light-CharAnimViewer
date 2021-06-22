
#include "Skeleton.h"
#include <src/master_CharAnim/Utility.h>

using namespace chara;

void Skeleton::init(const BVH& bvh)
{
	int i;
	float x, y, z;

	m_joints.resize(bvh.getNumberOfJoint());

	for (i = 0; i < bvh.getNumberOfJoint(); i++) {
		BVHJoint bvhj = bvh.getJoint(i);

		SkeletonJoint joint;
		joint.m_parentId = bvhj.getParentId();

		bvhj.getOffset(x, y, z);
		joint.m_l2w = Translation(Vector(x, y, z));

		if (joint.m_parentId != -1) {
			joint.m_l2w = joint.m_l2w * m_joints.at(joint.m_parentId).m_l2w;
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
	int i, j;
	float x, y, z,
		xt, yt, zt;


	Transform l2f;
	SkeletonJoint joint;
	//Transform  trans;

	int frame = std::abs(frameNumber) % bvh.getNumberOfFrame();
	for (i = 0; i < bvh.getNumberOfJoint(); i++) {
		BVHJoint bvhj = bvh.getJoint(i);
		joint.m_parentId = bvhj.getParentId();


		bvhj.getOffset(x, y, z);
		l2f = Translation(Vector(x, y, z));
		xt = 0, yt = 0, zt = 0;
		for (j = 0; j < bvhj.getNumberOfChannel(); j++) {
			BVHChannel chan = bvhj.getChannel(j);


			switch (chan.getAxis()) {
			case AXIS_X:
				if (chan.isRotation()) {
					l2f = l2f * Rotation(Vector(1, 0, 0), chan.getData(frame));
				}
				else {
					l2f = l2f * Translation(Vector(chan.getData(frame), 0, 0));
				}

				break;
			case AXIS_Y:
				if (chan.isRotation()) {
					l2f = l2f * Rotation(Vector(0, 1, 0), chan.getData(frame));
				}
				else {
					l2f = l2f * Translation(Vector(0, chan.getData(frame), 0));
				}
				break;
			case AXIS_Z:
				if (chan.isRotation()) {
					l2f = l2f * Rotation(Vector(0, 0, 1), chan.getData(frame));
				}
				else {
					l2f = l2f * Translation(Vector(0, 0, chan.getData(frame)));
				}
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





void Skeleton::setPoseInterpolation(const BVH& bvhSrc, int frameNbSrc, const BVH& bvhDst, int frameNbDst, float t) {

	int i, j;
	float x_s, y_s, z_s, x_d, y_d, z_d;
	float xt = 0, yt = 0, zt = 0;

	Transform a, b, l2f, transA, transB;
	SkeletonJoint joint;

	int frame_s = std::abs(frameNbSrc) % bvhSrc.getNumberOfFrame();
	int frame_d = std::abs(frameNbDst) % bvhDst.getNumberOfFrame();

	for (i = 0; i < bvhSrc.getNumberOfJoint(); i++) {
		BVHJoint bvhj_s = bvhSrc.getJoint(i);
		BVHJoint bvhj_d = bvhDst.getJoint(i);


		joint.m_parentId = bvhj_s.getParentId();


		bvhj_s.getOffset(x_s, y_s, z_s);
		bvhj_d.getOffset(x_d, y_d, z_d);


		a = Translation(Vector(x_s, y_s, z_s));
		b = Translation(Vector(x_d, y_d, z_d));
		l2f = Utility::slerp(a, b, t);

		xt = 0, yt = 0, zt = 0;
		for (j = 0; j < bvhj_s.getNumberOfChannel(); j++) {
			BVHChannel chan_s = bvhj_s.getChannel(j);
			BVHChannel chan_d = bvhj_d.getChannel(j);

			switch (chan_s.getAxis()) {
			case AXIS_X:
				if (chan_s.isRotation()) {
					transA = Rotation(Vector(1, 0, 0), chan_s.getData(frame_s));
					transB = Rotation(Vector(1, 0, 0), chan_s.getData(frame_d));

					l2f = l2f(Utility::slerp(transA, transB, t));
				}
				else {
					transA = Translation(Vector(chan_s.getData(frame_s), 0, 0));
					transB = Translation(Vector(chan_d.getData(frame_d), 0, 0));

					l2f = l2f(Utility::slerp(transA, transB, t));
				}

				break;
			case AXIS_Y:
				if (chan_s.isRotation()) {
					transA = Rotation(Vector(0, 1, 0), chan_s.getData(frame_s));
					transB = Rotation(Vector(0, 1, 0), chan_s.getData(frame_d));

					l2f = l2f(Utility::slerp(transA, transB, t));
				}
				else {
					transA = Translation(Vector(0, chan_s.getData(frame_s), 0));
					transB = Translation(Vector(0, chan_d.getData(frame_d), 0));

					l2f = l2f(Utility::slerp(transA, transB, t));
				}
				break;
			case AXIS_Z:
				if (chan_s.isRotation()) {
					transA = Rotation(Vector(0, 0, 1), chan_s.getData(frame_s));
					transB = Rotation(Vector(0, 0, 1), chan_s.getData(frame_d));

					l2f = l2f(Utility::slerp(transA, transB, t));
				}
				else {
					transA = Translation(Vector(0, 0, chan_s.getData(frame_s)));
					transB = Translation(Vector(0, 0, chan_d.getData(frame_d)));

					l2f = l2f(Utility::slerp(transA, transB, t));
				}
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

float Skeleton::distance(const Skeleton::CASkeleton& a, const Skeleton::CASkeleton& b)
{
	int i;
	float diff = 0;

	a.sk.setPose(a.bvh, a.frame);
	b.sk.setPose(b.bvh, b.frame);

	for (int i = 0; i < a.sk.numberOfJoint(); i++) {
		diff += distance2(a.sk.getJointPosition(i), b.sk.getJointPosition(i));
	}

	return diff;
}
