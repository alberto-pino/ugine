#include "Material.h"

#define BASE_TEXTURE_L 0
#define REFLECTION_TEXTURE_L 1
#define REFRACTION_TEXTURE_L 2
#define NORMAL_TEXTURE_L 3
#define CUBEMAP_TEXTURE_L 4
#define CUBEMAP_REFLECTION_TEXTURE_L 5
#define CUBEMAP_REFRACTION_TEXTURE_L 6
#define CUBEMAP_NORMAL_TEXTURE_L 7


Material::Material(const std::shared_ptr<Texture>& tex, const std::shared_ptr<Shader>& sh)
{
	texture = tex;
	shader = sh;
}

void Material::prepare()
{
	//Activamos el shader devuelto por getShader.

	std::shared_ptr<Shader> shader = getShader();

	shader->use();

	//Escribimos las variables uniformes necesarias.

	glm::mat4 MVP = State::projectionMatrix * State::viewMatrix * State::modelMatrix;
	glm::mat4 MV = State::viewMatrix * State::modelMatrix;
	glm::mat4 NormalM (MV);
	glm::transpose(glm::inverse(NormalM));


	shader->setMatrix(shader->getLocation("mvp"), MVP);
	shader->setMatrix(shader->getLocation("mv"), MV);
	shader->setMatrix(shader->getLocation("nmatrix"), NormalM);
	shader->setMatrix(shader->getLocation("modelmatrix"), State::modelMatrix);
	shader->setVec3(shader->getLocation("eyePos"), State::eyePos);


	int texbool = getShader()->getLocation("texbool");
	int texsampler = getShader()->getLocation("texsampler");
	int cubebool = getShader()->getLocation("cubebool");
	int cubesampler = getShader()->getLocation("cubesampler");

	int texRFLbool = getShader()->getLocation("texRFLbool");
	int texRFLsampler = getShader()->getLocation("texRFLsampler");
	int cubeRFLsampler = getShader()->getLocation("cubeRFLsampler");

	int texRFRbool = getShader()->getLocation("texRFRbool");
	int texRFRsampler = getShader()->getLocation("texRFRsampler");
	int texRFRcoef = getShader()->getLocation("texRFRcoef");
	int cubeRFRsampler = getShader()->getLocation("cubeRFRsampler");

	int texNMbool = getShader()->getLocation("texNMbool");
	int texNMsampler = getShader()->getLocation("texNMsampler");
	int cubeNMsampler = getShader()->getLocation("cubeNMsampler");


	// Check if there is a texture to be used
	if (texbool != -1)
	{
		if (texture)
		{
			shader->setInt(texbool, 1);

			if (texture->isCube())
			{
				shader->setInt(cubebool, 1);
				shader->setInt(cubesampler, CUBEMAP_TEXTURE_L);
				texture->bind(CUBEMAP_TEXTURE_L);
			}
			else
			{
				shader->setInt(cubebool, 0);
				shader->setInt(texsampler, BASE_TEXTURE_L);
				texture->bind(BASE_TEXTURE_L);
			}

		}
		else shader->setInt(texbool, 0);
	}

	// Check if there is a reflection texture to be used

	if (texRFLbool != -1)
	{
		if (reflectiontexture)
		{
			shader->setInt(texRFLbool, 1);

			if (reflectiontexture->isCube())
			{
				shader->setInt(cubeRFLsampler, CUBEMAP_REFLECTION_TEXTURE_L);
				reflectiontexture->bind(CUBEMAP_REFLECTION_TEXTURE_L);
			}
			else
			{
				shader->setInt(texRFLsampler, REFLECTION_TEXTURE_L);
				reflectiontexture->bind(REFLECTION_TEXTURE_L);
			}

		}
		else shader->setInt(texRFLbool, 0);
	}

	// Check if there is a refraction texture to be used

	if (texRFRbool != -1)
	{
		if (refractiontexture)
		{
			shader->setInt(texRFRbool, 1);
			shader->setFloat(texRFRcoef, refractionCoef);

			if (refractiontexture->isCube())
			{
				shader->setInt(cubeRFRsampler, CUBEMAP_REFRACTION_TEXTURE_L);
				refractiontexture->bind(CUBEMAP_REFRACTION_TEXTURE_L);
			}
			else
			{
				shader->setInt(texRFRsampler, REFRACTION_TEXTURE_L);
				refractiontexture->bind(REFRACTION_TEXTURE_L);
			}

		}
		else
		{
			shader->setInt(texRFRbool, 0);
			shader->setInt(texRFRcoef, 0);
		}
	}
	// Check if there is a reflection texture to be used

	if (texNMbool != -1)
	{
		if (normaltexture)
		{
			shader->setInt(texNMbool, 1);

			if (normaltexture->isCube())
			{
				shader->setInt(cubeNMsampler, CUBEMAP_NORMAL_TEXTURE_L);
				normaltexture->bind(CUBEMAP_NORMAL_TEXTURE_L);
			}
			else
			{
				shader->setInt(texNMsampler, NORMAL_TEXTURE_L);
				normaltexture->bind(NORMAL_TEXTURE_L);
			}

		}
		else shader->setInt(texNMbool, 0);
	}

	int loc = shader->getLocation("color");
	shader->setVec4(loc, color);

	loc = shader->getLocation("nLights");

	if (lighting)
	{
		shader->setInt(loc, State::lights.size());

		for (int i = 0; i < State::lights.size(); i++)
		{
			State::lights.at(i)->prepare(i, shader);
		}
	}
	else
	{
		shader->setInt(loc, 0);
	}

	loc = shader->getLocation("shininess");
	shader->setInt(loc, shininess);
	loc = shader->getLocation("ambient");
	shader->setVec3(loc, State::ambient);
	
	//Establecemos el modo de mezclado seleccionado.

	switch (blendingMode) {
		case Material::BlendMode::ALPHA:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case Material::BlendMode::ADD:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case Material::BlendMode::MUL:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		default:
			break;
	}

	// Activamos o desactivamos oclusión de caras traseras 
	(culling) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

	// Activamos o desactivamos la escritura en el depth buffer.
	glDepthMask(depthWrite);
}
