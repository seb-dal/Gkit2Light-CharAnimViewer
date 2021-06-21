#pragma once
#include "CharacterController.h"
#include <src/master_CharAnim/MotionGraph.h>



class SkeletonControler :public CharacterController {
	Skeleton sk;

	MotionGraph Mg;

	int pf_iddel, pf_walk, pf_run, pf_kick, pf_dance;
	BVH iddel, walk, run, kick, dance;
	// list BVH -> list idFrame ->
	//std::vector<std::vector<>>

public:
	SkeletonControler();

	Skeleton& getSkeleton();

	Transform& getPos();

	void update(const float dt);

};

