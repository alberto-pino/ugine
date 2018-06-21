#include "Light.h"

#include "State.h"


void Light::prepare(int index, std::shared_ptr<Shader>& shader) const
{
	std::string indexS = std::to_string(index);
	std::string name;
	int location;


	glm::vec4 position = glm::vec4(1, 1, 1, lightType);
	position = glm::rotate(glm::translate(glm::mat4(), m_position), glm::angle(m_quat), glm::axis(m_quat)) * position;
  	position = State::viewMatrix * position;

	glm::vec4 rotation;
	rotation = glm::vec4(m_euler, lightType);
	rotation = State::viewMatrix * rotation;


	name = "lights[" + indexS + "].position";
	location = shader->getLocation(name.c_str());
	shader->setVec4(location, position);

	name = "lights[" + indexS + "].rotation";
	location = shader->getLocation(name.c_str());
	shader->setVec4(location, rotation);

	name = "lights[" + indexS + "].linearAttenuation";
	location = shader->getLocation(name.c_str());
	shader->setFloat(location, linearAttenuation);

	name = "lights[" + indexS + "].lightColor";
	location = shader->getLocation(name.c_str());
	shader->setVec4(location, glm::vec4(lightColor, 1.0f));


}
