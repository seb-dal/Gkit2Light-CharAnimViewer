/****************************************************************************
Copyright (C) 2010-2020 Alexandre Meyer

This file is part of Simea.

Simea is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Simea is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Simea.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef PARTICLES_H
#define PARTICLES_H

#include <iostream>
#include <vector>

#include "vec.h"

class Particle
{
public:

	Particle() {
		m_radius = 10 + rand() % 16;
		m_mass = m_radius / 10;      // 1kg


		m_p.x = rand() % 600 - 200;
		m_p.y = m_radius + 5 + rand() % 200;
		m_p.z = rand() % 600 - 200;

	}

	Particle(float m_radius, float m_mass, Point pos) :m_p(pos), m_radius(m_radius), m_mass(m_mass) {}
	Particle(Particle& p) :m_p(p.m_p), m_radius(p.m_radius), m_mass(p.m_mass), m_v(p.m_v), m_f(p.m_f), fixed(p.fixed) {}

	Particle& operator=(Particle& p) {
		m_p = p.m_p;
		m_radius = p.m_radius;
		m_mass = p.m_mass;
		m_v = p.m_v;
		m_f = p.m_f;
		fixed = p.fixed;

		return *this;
	}

	void update(const float dt = 0.1f)		// advect
	{
		if (fixed) {
			m_f = Vector(0, 0, 0);
			return;
		}

		//TODO
		if (m_mass > 0) {
			// mise à jour de la vitesse
			m_v = m_v + (m_f / m_mass) * dt;
			// mise à jour de la position
			m_p = m_p + m_v * dt;
			// remise à 0 de la force
			m_f = Vector(0, 0, 0);
		}
	}

	//! Collision with the ground (y=0)
	void groundCollision()
	{
		if (m_radius < 0) return;
		if (m_p.y < m_radius) {
			m_p.y = m_radius;
			m_f.y = m_f.y * (-0.98);
			m_v = m_v * (1 - frictionSol);
			m_v.y = 0;
		}
	}

	//! Collision with any point p of radius radius (this will be used for kicking with the character's bones)
	void collision(const Point& p, const float radius) {
		if (m_radius < 0) return;

		if (distance(p, m_p) <= radius + m_radius) {
			addForce(Vector(m_p - p) * 0.75);
		}
	}


	void collision(Particle& pa) {
		if(this == &pa)return;
		if (m_radius < 0) return;
		if (pa.m_radius < 0) return;

		if (distance(pa.position(), m_p) <= pa.radius() + m_radius) {
			float l1 = length(pa.m_f);
			float l2 = length(m_f);

			Vector f = m_f;
			m_f = pa.m_f * 0.5;
			pa.m_f = f * 0.5;

			Vector dir = normalize(Vector(m_p - pa.position()));

			addForce(l1 * 0.5 * dir);
			pa.addForce(l2 * 0.5 * (-dir));
		}
	}

	//! add force to the particles
	void addForce(const Vector& force) {
		if (fixed) return;

		m_f = m_f + force;
	}


	//! Apply gravity
	void addEarthGravity() {
		// apply gravity, call addForce
		addForce(Vector(0.f, -m_mass * 9.81f, 0.f));
	}

	const Point& position() const { return m_p; }
	Point& position() { return m_p; }

	const Vector& force() const { return m_f; }
	Vector& force() { return m_f; }

	const Vector& vitesse() const { return m_v; }
	Vector& vitesse() { return m_v; }

	void fixParticle() { fixed = true; }

	float radius() const { return m_radius; }

	friend std::ostream& operator<<(std::ostream& o, const Particle& p)
	{
		o << " p=(" << p.m_p.x << "," << p.m_p.y << ") v=(" << p.m_v.x << "," << p.m_p.y << ") m=" << p.m_mass << std::endl;
		return o;
	}

protected:
	const float frictionSol = 0.025;

	Point m_p;				//!< position
	float m_radius;			//!< radius
	Vector m_v;				//!< velocity m/s
	Vector m_f;				//!< force in N
	float m_mass;			//!< mass in kg
	bool fixed = false;

};

#endif
