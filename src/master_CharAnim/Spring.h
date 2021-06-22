#pragma once

#include <src/master_CharAnim/Particle.h>

class Spring {
public:
	Spring(Particle* a, Particle* b, float l0, float k, float v) :a(a), b(b), k(k), l0(l0), v(v) {}

	void addForce(float dt) {
		// 𝑝𝐴 − 𝑝B
		Vector pampb = a->position() - b->position();

		// ||𝑝𝐴 − 𝑝B||
		float ABSpampb = std::max(distance(a->position(), b->position()), 0.1f);

		// 𝐾(||𝑝𝐴 − 𝑝𝐵||) − 𝑙0
		float k_ABSpampb = k * (ABSpampb - l0);

		// (𝑝𝐴−𝑝𝐵) / ||𝑝𝐴−𝑝𝐵||
		Vector nab = pampb / ABSpampb;

		// 𝜈(𝑣𝐴 − 𝑣𝐵) ∙ 𝑛𝐴B
		float vamvb = v * dot(a->vitesse() - b->vitesse(), nab);

		// 𝑓𝐵→A = ((𝐾(||𝑝𝐴 − 𝑝𝐵||) − 𝑙0) + 𝜈(𝑣𝐴 − 𝑣𝐵) ∙ 𝑛𝐴B) × 𝑛𝐴𝐵
		Vector fba = (k_ABSpampb + vamvb) * dt * nab;

		a->addForce(-fba);
		b->addForce(fba);
	}

	Particle* getA() { return a; }
	Particle* getB() { return b; }

private:
	Particle* a, * b;

	float l0; // longueur au repos
	float k; // coeff raideur
	float v; // Amortissement
};

