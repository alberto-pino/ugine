#include "Emitter.h"

Emitter::Emitter(const Material& mat, bool autofade)
{
	material = mat;
	autoFade = autofade;
}

void Emitter::update(float deltaTime)
{

	for (int i = static_cast<int>(particles.size()) - 1; i >= 0; --i)
	{
		particles.at(i).update(deltaTime);

		if (particles.at(i).getRemainingLifetime() <= 0.0f)
			particles.erase(particles.begin() + i);
	}

	if (emitting)
	{
		float randN = glm::linearRand(minRateRange, maxRateRange) * deltaTime + particlesToEmit;
		int particlesToAdd = static_cast<int>(randN) / 1;
		particlesToEmit = randN - particlesToAdd;


		for (int i = 0; i < particlesToAdd; i++)
		{

			glm::vec3 particleVel = glm::linearRand(minVelRange, maxVelRange);
			float particleSpin = glm::linearRand(minSpinRange, maxSpinRange);
			float particleScale = glm::linearRand(minScaleRange, maxScaleRange);
			float particleLifetime = glm::linearRand(minLifetimeRange, maxLifetimeRange);
			glm::vec4 particleColor = glm::linearRand(minColorRange, maxColorRange);

			material.setColor(particleColor);

			Particle particle(material, particleVel, particleSpin, particleLifetime, autoFade);
			particle.setScale(glm::vec3(particleScale, particleScale, 1.0f));
			particle.setPosition(m_position);

			particles.push_back(particle);
		}
	}

}

void Emitter::draw() 
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles.at(i).draw();
	}
}