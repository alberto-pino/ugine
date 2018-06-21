#pragma once

#include "common.h"
#include "entity.h"
#include "Material.h"
#include "Particle.h"


class Emitter : public Entity {

public:

	Emitter(const Material& mat, bool autofade = true);

	void			setRateRange(float min, float max) { minRateRange = min; maxRateRange = max; };
	void			setVelocityRange(const glm::vec3& min, const glm::vec3& max) { minVelRange = min; maxVelRange = max; };
	void			setSpinVelocityRange(float min, float max) { minSpinRange = min; maxSpinRange = max; };
	void			setScaleRange(float min, float max) { minScaleRange = min; maxScaleRange = max; };
	void			setLifetimeRange(float min, float max) { minLifetimeRange = min; maxLifetimeRange = max; };
	void			setColorRange(const glm::vec4& min, const glm::vec4& max) { minColorRange = min; maxColorRange = max; };

	void			emit(bool enable) { emitting = enable; };
	bool			isEmitting() { return emitting; };

	virtual void	update(float deltaTime) override;
	virtual void	draw() override;

	int				getNumberParticles() { return particles.size(); };

private:
	Material material;
	bool autoFade;
	bool emitting;

	float minRateRange;
	float maxRateRange;
	glm::vec3 minVelRange;
	glm::vec3 maxVelRange;
	float minSpinRange;
	float maxSpinRange;
	float minScaleRange;
	float maxScaleRange;
	float minLifetimeRange;
	float maxLifetimeRange;
	glm::vec4 minColorRange;
	glm::vec4 maxColorRange;

	std::vector<Particle> particles;

	float particlesToEmit = 0.0f;

};