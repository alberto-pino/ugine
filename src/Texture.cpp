
#include "Texture.h"

#include "../lib/stb-image/stb_image.h"

std::shared_ptr<Texture> Texture::load(const char* filename)
{
	int imgY;
	int imgX;
	GLuint textureId;

	stbi_set_flip_vertically_on_load(true);
	stbi_uc* image = stbi_load(filename, &imgY, &imgX, nullptr, 4);

	if (!image) return nullptr;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgX, imgY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);


	std::shared_ptr<Texture> texture(new Texture(textureId, imgY, imgX), supr);
	stbi_image_free(image);

	return texture;
}

std::shared_ptr<Texture> Texture::load(	const char* left, const char* right,
										const char* front, const char* back,
										const char* top, const char* bottom)
{
	int imgY;
	int imgX;
	GLuint textureId;

	stbi_set_flip_vertically_on_load(false);
	stbi_uc* imagel = stbi_load(left, &imgY, &imgX, nullptr, 4);
	if (!imagel) return nullptr;
	stbi_uc* imager = stbi_load(right, &imgY, &imgX, nullptr, 4);
	if (!imager) return nullptr;
	stbi_uc* imagef = stbi_load(front, &imgY, &imgX, nullptr, 4);
	if (!imagef) return nullptr;
	stbi_uc* imageb = stbi_load(back, &imgY, &imgX, nullptr, 4);
	if (!imageb) return nullptr;
	stbi_uc* imaget = stbi_load(top, &imgY, &imgX, nullptr, 4);
	if (!imaget) return nullptr;
	stbi_uc* imagebt = stbi_load(bottom, &imgY, &imgX, nullptr, 4);
	if (!imagebt) return nullptr;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, imgX, imgY, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagel);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, imgX, imgY, 0, GL_RGBA, GL_UNSIGNED_BYTE, imager);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, imgX, imgY, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagef);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, imgX, imgY, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageb);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, imgX, imgY, 0, GL_RGBA, GL_UNSIGNED_BYTE, imaget);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, imgX, imgY, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagebt);

	std::shared_ptr<Texture> texture(new Texture(textureId, imgY, imgX, true), supr);
	
	stbi_image_free(imagel);
	stbi_image_free(imager);
	stbi_image_free(imagef);
	stbi_image_free(imageb);
	stbi_image_free(imaget);
	stbi_image_free(imagebt);

	return texture;
}

bool Texture::isCube() const
{
	return cube;
}

void Texture::bind(size_t layer = 0) const
{
glActiveTexture(GL_TEXTURE0 + layer);
if (cube)	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
else		glBindTexture(GL_TEXTURE_2D, textureId); 
}