#pragma once

#include "common.h"
#include "Entity.h"
#include "Shader.h"

class Light : public Entity
{

public:

	enum Type { DIRECTIONAL = 0, POINT };

	Light(glm::vec3 pos, Type type, glm::vec3 color, float att, glm::vec3 direction) : 
		lightType(type), lightColor(color), linearAttenuation(att) {
		m_position = pos;
		m_euler = direction;
	};

	Type			getType() const { return lightType; }
	void			setType(Type type) { lightType = type; }
	const glm::vec3& getColor() const { return lightColor; }
	void			setColor(const glm::vec3& color) { lightColor = color; }
	float			getLinearAttenuation() const { return linearAttenuation; }
	void			setLinearAttenuation(float att) { linearAttenuation = att; }
	void			prepare(int index, std::shared_ptr<Shader>& shader) const;


private:
	Type lightType;
	glm::vec3 lightColor;
	float linearAttenuation;

};