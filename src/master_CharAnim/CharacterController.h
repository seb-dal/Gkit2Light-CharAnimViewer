#pragma once

#include <src/gKit/mat.h>
#include "BVH.h"
#include "Skeleton.h"

using namespace chara;

class CharacterController
{
public:
	CharacterController();

	void update(const float dt);

	//void turn(const Transform& transf_v);  // pas forcement nécessaire
	void turnXZ(const float& rot_angle_v);
	void accelerate(const float& speed_inc);
	void setVelocityMax(const float vmax);

	void setPosition(const Vector& p);
	void setVelocityNorm(const float v);

	const Vector position() const;
	Vector position(float pos);
	const Transform direction() const;
	float velocity() const;



protected:
	Transform m_ch2w;   // matrice du character vers le monde
						// le personnage se déplace vers X
						// il tourne autour de Y
						// Z est sa direction droite

	float m_v;          // le vecteur vitesse est m_v * m_ch2w * Vector(1,0,0)
	float m_vMax;       // ne peut pas accélérer plus que m_vMax

	const float m_MAX_V = 6; //Vitesse maximal
	const float m_accel = 0.025; // Accéleration
	const float m_rotatSpeed = 3; // vitesse de rotation
};

