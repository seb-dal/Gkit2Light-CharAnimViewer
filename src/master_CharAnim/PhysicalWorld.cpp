
#include <PhysicalWorld.h>

#include <CharAnimViewer.h>


void PhysicalWorld::update(const float dt)
{
	int i, j;
	for (i = 0; i < m_part.size(); ++i)
	{
		// i_eme particule update
		m_part[i].update(dt);
		// i_eme particule collision
		m_part[i].groundCollision();
		// i_eme particule add gravirty
		m_part[i].addEarthGravity();

		for (j = i + 1; j < m_part.size(); j++) {
			m_part[i].collision(m_part[j]);
		}
	}

	for (Spring& sp : tissus.getSprings()) {
		sp.addForce(0.05);
	}


	Array2D<Particle> particles = tissus.getParticules();
	int max = particles.sizeTotal();
	i = 0;
	for (Particle& parti : tissus.getParticules().get()) {
		parti.update(0.05);
		parti.groundCollision();
		parti.addEarthGravity();

		for (j = 0; j < max; j++) {
			if (i != j) {
				parti.collision(particles[j]);
			}
		}

		for (j = 0; j < m_part.size(); ++j) {
			parti.collision(m_part[j]);
		}
		i++;
	}

}


void PhysicalWorld::draw()
{
	int i, max;
	max = particlesCount();
	for (i = 0; i < max; ++i) {
		if (m_part[i].radius() > 0)
			CharAnimViewer::singleton().draw_sphere(m_part[i].position(), m_part[i].radius());
	}


	Array2D<Particle> parti = tissus.getParticules();
	max = parti.sizeTotal();
	for (i = 0; i < max; ++i) {
		if (parti(i).radius() > 0)
			CharAnimViewer::singleton().draw_sphere(parti[i].position(), parti[i].radius());
	}


	std::vector<Spring> ressors = tissus.getSprings();
	max = ressors.size();
	for (i = 0; i < max; ++i) {
		CharAnimViewer::singleton().draw_cylinder(ressors.at(i).getA()->position(), ressors.at(i).getB()->position());
	}

}


void PhysicalWorld::collision(const Point& p, const float radius)
{
	int i;
	for (i = 0; i < m_part.size(); ++i) {
		m_part[i].collision(p, radius);
	}

	for (Particle& parti : tissus.getParticules().get()) {
		parti.collision(p, radius);
	}

}
