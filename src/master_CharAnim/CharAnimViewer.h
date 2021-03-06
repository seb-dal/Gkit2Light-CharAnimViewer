
#ifndef _CHARANIMVIEWER_H
#define _CHARANIMVIEWER_H


#include "quaternion.h"
#include "Viewer.h"
#include "BVH.h"
#include "Skeleton.h"
#include "TransformQ.h"

#include <PhysicalWorld.h>
#include <src/master_CharAnim/SkeletonControler.h>

class CharAnimViewer : public Viewer
{
public:
	CharAnimViewer();

	int init();
	int render();
	int update(const float time, const float delta);

	static CharAnimViewer& singleton() { return *psingleton; }

protected:
	void bvhDrawRec(const chara::BVHJoint& bvh, const Transform& f2w, int f);

	chara::BVH m_bvh;
	int m_frameNumber;

	Skeleton m_ske;

	PhysicalWorld m_world;

	SkeletonControler skc;

	void draw_skeleton(const Skeleton&);

	void draw_skeleton(const Skeleton& sk, const Transform& pos);

	void collision_skeleton(const Skeleton& sk, const Transform& pos);

private:
	static CharAnimViewer* psingleton;
};



#endif
