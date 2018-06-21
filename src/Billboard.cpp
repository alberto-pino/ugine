#include "Billboard.h"

Billboard::Billboard(const Material& mat)
{
	material = mat;
	/*//if (material.getTexture() != nullptr)
		size = material.getTexture()->getSize();
	//else
		//size = { 1,1 };*/
	size = material.getTexture()->getSize();
	material.setDepthWrite(false);

	spin = 0;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);

	std::vector<Vertex> vertexvec;
	Vertex vertex;

	vertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);

	vertex.position = glm::vec3(-0.5f, -0.5f, 0.0f);
	vertex.texture = glm::vec2(0, 0);
	vertexvec.push_back(vertex);

	vertex.position = glm::vec3(0.5f, -0.5f, 0.0f);
	vertex.texture = glm::vec2(1, 0);
	vertexvec.push_back(vertex);

	vertex.position = glm::vec3(0.5f, 0.5f, 0.0f);
	vertex.texture = glm::vec2(1, 1);
	vertexvec.push_back(vertex);

	vertex.position = glm::vec3(-0.5f, 0.5f, 0.0f);
	vertex.texture = glm::vec2(0, 1);
	vertexvec.push_back(vertex);

	std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };

	//buffer = Buffer::create(vertexvec, indices);
	//std::shared_ptr<Buffer> buffer(new Buffer(vertexvec, indices));

	std::shared_ptr<Buffer> b(new Buffer(vertexvec, indices));
	buffer = b;
}

void Billboard::draw()
{
	// Get the transpose viewMatrix to generate the model matrix of the Billboard
	glm::mat4 newModelMatrix = glm::transpose(State::viewMatrix);

	// Reinitialize the needed values -> Position is set
	newModelMatrix[0][3] = 0;
	newModelMatrix[1][3] = 0;
	newModelMatrix[2][3] = 0;
	newModelMatrix[3] = glm::vec4(getPosition(), 1.0f);

	// Rotate and Scale the Billboard
	newModelMatrix = glm::rotate(newModelMatrix, glm::radians(spin), glm::vec3(0, 0, 1));
	//glm::vec3 billboardScale =  * m_scale;
	newModelMatrix = glm::scale(newModelMatrix, glm::vec3(size, 1)*getScale());
	State::modelMatrix = newModelMatrix;

	//Set the parameters to draw the Billboard
	material.prepare();
	//std::shared_ptr<Shader> shader = material.getShader();
	buffer->draw(material.getShader());
}
