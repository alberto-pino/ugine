#pragma once

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION

#include "common.h"

#include "../lib/glew/glew.h"

class Texture
{
public:


	static std::shared_ptr<Texture>	load(const char* filename);

	static std::shared_ptr<Texture> load(const char* left, const char* right,
										 const char* front, const char* back,
										 const char* top, const char* bottom);

	bool							isCube() const;

	uint32_t						getId() const { return textureId; }
	const glm::ivec2&				getSize() const { glm::ivec2 size = glm::ivec2(width, height); return size; }

	void							bind(size_t layer) const;


protected:
	Texture (GLuint textureId, int height, int width, bool cubeb=false) : textureId(textureId), height(height), width(width), cube(cubeb) {}
	~Texture () {};
	static void supr (Texture* p) {delete p;}

	uint32_t textureId;

	int height;
	int width;
	bool cube = false;
};