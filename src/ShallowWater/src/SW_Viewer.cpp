#include "../include/SW_Viewer.h"
#include <src/ShallowWater/include/GrilleBuilder.h>

int SW_Viewer::init() {
	water = Mesh(GL_TRIANGLES);
	Sol = Mesh(GL_TRIANGLES);
	GrilleBuilder::build(water, -(sizeX / 2), (sizeX / 2), -(sizeY / 2), (sizeY / 2), 1);
	GrilleBuilder::build(Sol, -(sizeX / 2), (sizeX / 2), -(sizeY / 2), (sizeY / 2), 1);

	swater.init(sizeX, sizeY);
	swater.drawSol(Sol, -(sizeX / 2), (sizeX / 2), -(sizeY / 2), (sizeY / 2), 1);

	Point min, max;
	water.bounds(min, max);
	m_camera.lookat(min, max);
	m_camera.rotation(0, 45);

	//GL
	glClearColor(0.5f, 0.5f, 0.9f, 1);
	glEnable(GL_DEPTH_TEST);

	return 0;
}

int SW_Viewer::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl.camera(m_camera);
	gl.draw(Sol);
	gl.draw(water);

	return 1;
}

static const float v60FPS = 1000 / 60;
int SW_Viewer::update(const float time, const float delta) {
	swater.computeOneStep();
	swater.draw(water, -(sizeX / 2), (sizeX / 2), -(sizeY / 2), (sizeY / 2), 1);


	control((delta / v60FPS));
	return 0;
}

void SW_Viewer::control(const float delta) {
	// recupere les mouvements de la souris pour deplacer la camera, cf tutos/tuto6.cpp
	int mx, my;
	unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
	// deplace la camera
	if ((mb & SDL_BUTTON(1)) && (mb & SDL_BUTTON(3)))                 // le bouton du milieu est enfonce
		m_camera.translation((float)mx / (float)window_width(), (float)my / (float)window_height());         // deplace le point de rotation
	else if (mb & SDL_BUTTON(1))                      // le bouton gauche est enfonce
		m_camera.rotation(mx, my);       // tourne autour de l'objet
	else if (mb & SDL_BUTTON(3))                 // le bouton droit est enfonce
		m_camera.move(my);               // approche / eloigne l'objet

	float step = 2 / delta, stepT = 0.02 / delta;

	if (key_state(SDLK_PAGEUP) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT))) { m_camera.translation(0, stepT); }
	if (key_state(SDLK_PAGEDOWN) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT))) { m_camera.translation(0, -stepT); }
	if (key_state(SDLK_LEFT) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT))) { m_camera.translation(stepT, 0); }
	if (key_state(SDLK_RIGHT) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT))) { m_camera.translation(-stepT, 0); }
	if (key_state(SDLK_UP) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT))) { m_camera.move(step); }
	if (key_state(SDLK_DOWN) && (!key_state(SDLK_LCTRL)) && (!key_state(SDLK_LALT))) { m_camera.move(-step); }

	//NumPad Camera Control

	if (key_state(SDLK_KP_8)) { m_camera.translation(0, stepT); }
	if (key_state(SDLK_KP_2)) { m_camera.translation(0, -stepT); }
	if (key_state(SDLK_KP_4)) { m_camera.translation(stepT, 0); }
	if (key_state(SDLK_KP_6)) { m_camera.translation(-stepT, 0); }
	if (key_state(SDLK_KP_PLUS)) { m_camera.move(step); }
	if (key_state(SDLK_KP_MINUS)) { m_camera.move(-step); }
	if (key_state(SDLK_KP_7)) { m_camera.rotation(step, 0); }
	if (key_state(SDLK_KP_1)) { m_camera.rotation(-step, 0); }
	if (key_state(SDLK_KP_9)) { m_camera.rotation(0, step); }
	if (key_state(SDLK_KP_3)) { m_camera.rotation(0, -step); }
	if (key_state(SDLK_KP_5)) {
		Point min, max;
		water.bounds(min, max);
		m_camera.lookat(min, max);
		m_camera.rotation(0, 45);
	}
}
