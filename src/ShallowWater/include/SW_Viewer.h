#pragma once
#include <../gKit/app.h>
#include <src/gKit/mesh.h>
#include <src/gKit/orbiter.h>
#include <src/gKit/draw.h>
#include <src/ShallowWater/include/ShallowWater.h>

class SW_Viewer :public App {
public:
	SW_Viewer() :App(1280, 720) {}

	int init();
	int render();
	int update(const float time, const float delta);


	void control(const float delta);
	int quit() { return 1; }
private:
	int sizeX = 200, sizeY = 200;

	Mesh water;
	Mesh Sol;
	ShallowWater swater;

private:
	Orbiter m_camera;
	DrawParam gl;
};
