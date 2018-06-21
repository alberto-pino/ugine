#pragma once

#include "common.h"
#include "shader.h"
#include "vertex.h"

class Buffer {
public:
	Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
	~Buffer();
	void draw(const std::shared_ptr<Shader> & shader) const;

protected:
	std::array<uint32_t, 2>	m_buffers;
	size_t					m_numIndices;

	char error[1024];

};