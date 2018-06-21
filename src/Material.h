#pragma once

#include "common.h"
#include "Shader.h"
#include "Texture.h"
#include "State.h"


class Material
{
public:

	enum BlendMode { ALPHA = 0, ADD, MUL };

	Material(const std::shared_ptr<Texture>& tex = nullptr, const std::shared_ptr<Shader>& sh = nullptr);

	const std::shared_ptr<Shader>& getShader() const {
		if (!shader) return State::defaultShader;
		 else return shader;
	}
	std::shared_ptr<Shader>& getShader() {
		if (!shader) return State::defaultShader;
		else return shader;
	}
	void setShader(const std::shared_ptr<Shader>& sh) { shader = sh; }


	const glm::vec4&	getColor() const { return color; }
	void			setColor(const glm::vec4& c) { color = c; }

	uint8_t		getShininess() const { return shininess; }
	void			setShininess(uint8_t shin) { shininess = shin; }

	BlendMode	getBlendMode() const { return blendingMode; };
	void		setBlendMode(BlendMode blendMode) { blendingMode = blendMode; };
	bool		getLighting() const { return lighting; };
	void		setLighting(bool enable) { lighting = enable; };
	bool		getCulling() const { return culling; };
	void		setCulling(bool enable) { culling = enable; };
	bool		getDepthWrite() const { return depthWrite; };
	void		setDepthWrite(bool enable) { depthWrite = enable; };

	const std::shared_ptr<Texture>& getTexture() const { return texture; }
	void setTexture(const std::shared_ptr<Texture>& tex) { texture = tex; }

	const std::shared_ptr<Texture>& getReflectionTexture() const { return reflectiontexture; };
	void setReflectionTexture(const std::shared_ptr<Texture>& tex) { reflectiontexture = tex; };

	const std::shared_ptr<Texture>& getRefractionTexture() const { return refractiontexture; };
	void setRefractionTexture(const std::shared_ptr<Texture>& tex) { refractiontexture = tex; };

	const std::shared_ptr<Texture>& getNormalTexture() const { return normaltexture; };
	void setNormalTexture(const std::shared_ptr<Texture>& tex) { normaltexture = tex; };

	float getRefractionCoef() const { return refractionCoef; };
	void setRefractionCoef(float coef) { refractionCoef = coef; };

	void prepare();

private:
	std::shared_ptr<Texture> texture;
	std::shared_ptr<Texture> reflectiontexture;
	std::shared_ptr<Texture> refractiontexture;
	std::shared_ptr<Texture> normaltexture;

	std::shared_ptr<Shader> shader;
	glm::vec4 color;
	uint8_t shininess;
	float refractionCoef;

	BlendMode blendingMode;
	bool lighting=true;
	bool culling=true;
	bool depthWrite=true;
};