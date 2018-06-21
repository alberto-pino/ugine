#pragma once

#include "common.h"
#include "Billboard.h"

class Particle : public Billboard {

public:

	Particle(
		const Material& mat,
		const glm::vec3& vel,
		float spinVel,
		float lifetime,
		bool autofade);

	float			getRemainingLifetime() const { return lifetime; };
	virtual void	update(float deltaTime) override;

private:

	glm::vec3 velocity;
	float initialLifetime;
	bool timeFading;

	float lifetime;
};