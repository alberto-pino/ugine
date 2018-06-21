#include "Particle.h"

Particle::Particle(	const Material& mat, const glm::vec3& vel, float spinVel, float lifetime, bool autofade) : Billboard(mat)
{
	spin = spinVel;
	this->lifetime = lifetime;
	initialLifetime = lifetime;
	timeFading= autofade;
	velocity = vel;
}


void Particle::update(float deltaTime)
{
	lifetime = lifetime - deltaTime;
	move(velocity * deltaTime);
	m_euler = m_euler + glm::vec3(0, 0, (spin * deltaTime));

	if (timeFading)
	{
		float fade = lifetime / initialLifetime;

		material.setColor(glm::vec4(material.getColor().x, material.getColor().y, material.getColor().z, fade));
	}

}
