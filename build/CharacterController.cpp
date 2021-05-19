#include "CharacterController.h"
#include "window.h"

static Vector Extract_col(const Transform& t, int col) {
	return Vector(t.m[2][col], t.m[1][col], t.m[0][col]);
}

static Vector Extract_translate(const Transform& t) {
	return Extract_col(t, 3);
}


static Vector Extract_Scale(const Transform& t) {
	return Vector(
		length(Extract_col(t, 0)),
		length(Extract_col(t, 1)),
		length(Extract_col(t, 2))
	);
}

static Transform Extract_Rotation(const Transform& t) {
	Vector scale = Extract_Scale(t);
	return Transform(
		Extract_col(t, 0) / scale(0),
		Extract_col(t, 1) / scale(1),
		Extract_col(t, 2) / scale(2),
		Vector(0, 0, 0)
	);
}


CharacterController::CharacterController() {
	m_v = 0;
	m_vMax = 0;
}

static bool upPush = false;
static bool downPush = false;
static bool leftPush = false;
static bool rightPush = false;
static float rot = 0;

void CharacterController::update(const float dt) {
	const float r = dt / 10;

	float s = 1;
	//Saut
	if (key_state(SDLK_SPACE)) {
		s = 0.25f;
	}

	if (key_state('z') || key_state('Z')) {
		if (!upPush && key_event().type == SDL_KEYDOWN) {
			if (m_vMax < 5)
				accelerate(1);
			upPush = true;
		}
		else {
			upPush = false;
		}
	}
	if (key_state('s') || key_state('S')) {
		if (m_vMax > 0)
			accelerate(-1);
	}

	if (key_state('d') || key_state('D')) {
		turnXZ(r * m_rotatSpeed);
	}
	if (key_state('q') || key_state('Q')) {
		turnXZ(r * (-m_rotatSpeed));
	}

	if (m_v < m_vMax) {
		setVelocityNorm(m_v + m_accel * r * s);
	}
	else if (m_v > m_vMax) {
		setVelocityNorm(m_v - m_accel * r * s);
	}

	setPosition(direction()(Vector(velocity(), 0, 0)));
	std::cout << "pos : " << position() << std::endl
		<< "veloc : " << velocity() << std::endl
		<< "dir : " << direction() << std::endl
		<< "rot : " << rot << std::endl;
}

void CharacterController::turnXZ(const float& rot_angle_v) {
	m_ch2w = m_ch2w(Rotation(Vector(0, -1, 0), rot_angle_v));
	rot += rot_angle_v;
}

void CharacterController::accelerate(const float& speed_inc) {
	m_vMax = std::max(m_vMax + speed_inc, 0.f);
}

void CharacterController::setVelocityMax(const float vmax) {
	m_vMax = vmax;
}

void CharacterController::setPosition(const Vector& p) {
	m_ch2w = Translation(p)(m_ch2w);
}


void CharacterController::setVelocityNorm(const float v) {
	m_v = std::max(v, 0.f);
}

const Vector CharacterController::position() const {
	return Extract_translate(m_ch2w);
}

Vector CharacterController::position(float pos) {
	return Extract_translate(Translation(direction()(Vector(pos, 0, 0)))(m_ch2w));
}


const Transform CharacterController::direction() const {
	return Extract_Rotation(m_ch2w);
}

float CharacterController::velocity() const {
	return m_v;
}
