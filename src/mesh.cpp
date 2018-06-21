#include "mesh.h"
#include "state.h"

#include "Material.h"
#include "Texture.h"
#include <iostream>



void Mesh::draw() {
	for (auto& pair : m_pairs) {

		std::shared_ptr<Shader> shader = pair.second.getShader();

		//Activamos el shader, preparamos el material y usamos draw en el buffer
		shader->use();
		pair.second.prepare();
		pair.first->draw(shader);
	}
}

inline std::string extractPath(std::string filename) {
	while (filename.find('\\') != std::string::npos)
		filename.replace(filename.find('\\'), 1, 1, '/');
	size_t pos = filename.rfind('/');
	if (pos == std::string::npos) return "";
	filename = filename.substr(0, pos);
	if (filename.size() > 0) filename += '/';
	return filename;
}


template<typename T>
std::vector<T> splitString(const std::string& str, char delim) {
	std::vector<T> elems;
	if (str != "") {
		std::stringstream ss(str);
		std::string item;
		while (std::getline(ss, item, delim)) {
			T value;
			std::stringstream ss2(item);
			ss2 >> value;
			elems.push_back(value);
		}
	}
	return elems;
}


std::shared_ptr<Mesh> Mesh::load( const char* filename, const std::shared_ptr<Shader>& shader)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	bool includesTexture = false;
	bool includescoordsTexture = false;
	bool includesRFLTexture = false;
	bool includesRFRTexture = false;
	bool includesNMTexture = false;
	bool includesNormals = false;
	bool includesTangent = false;
	bool disableLighting = false;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);


	if (result) {
		// Cargado correctamente, podemos analizar su contenido ...


		pugi::xml_node meshNode = doc.child("mesh");
		pugi::xml_node buffersNode = meshNode.child("buffers");

		for (pugi::xml_node bufferNode = buffersNode.child("buffer");
			bufferNode;
			bufferNode = bufferNode.next_sibling("buffer"))
		{
			// Iteramos por todos los buffers
			pugi::xml_node materialNode = bufferNode.child("material");

			std::string textureName = materialNode.child("texture").text().as_string();
			std::vector<std::string> textureList;
			textureList = splitString<std::string>(textureName, ',');
			std::shared_ptr<Texture> texture;

			if (textureList.size() == 1)
			{
				std::string fullTextureName = extractPath(filename) + textureName;
				texture = Texture::load(fullTextureName.c_str());
				includesTexture = true;
			}
			else if (textureList.size() == 6)
			{
				std::vector<std::string> fullTextureName;
				for (int i = 0; i < 6; i++)
				{
					fullTextureName.push_back( extractPath(filename) + textureList[i]);
				}
				texture = Texture::load(fullTextureName[0].c_str(), fullTextureName[1].c_str(),
										fullTextureName[2].c_str(), fullTextureName[3].c_str(), 
										fullTextureName[4].c_str(), fullTextureName[5].c_str());
				includesTexture = true;
				disableLighting = true;
			}

			std::string textureNameRFL = materialNode.child("reflect_texture").text().as_string();
			std::vector<std::string> textureListRFL;
			textureListRFL = splitString<std::string>(textureNameRFL, ',');
			std::shared_ptr<Texture> textureRFL;

			if (textureListRFL.size() == 1)
			{
				std::string fullTextureNameRFL = extractPath(filename) + textureNameRFL;
				textureRFL = Texture::load(fullTextureNameRFL.c_str());
				includesTexture = true;
			}
			else if (textureListRFL.size() == 6)
			{
				std::vector<std::string> fullTextureNameRFL;
				for (int i = 0; i < 6; i++)
				{
					fullTextureNameRFL.push_back(extractPath(filename) + textureListRFL[i]);
				}
				textureRFL = Texture::load(fullTextureNameRFL[0].c_str(), fullTextureNameRFL[1].c_str(),
					fullTextureNameRFL[2].c_str(), fullTextureNameRFL[3].c_str(),
					fullTextureNameRFL[4].c_str(), fullTextureNameRFL[5].c_str());
				includesTexture = true;
			}

			std::string textureNameRFR = materialNode.child("refract_texture").text().as_string();
			std::vector<std::string> textureListRFR;
			textureListRFR = splitString<std::string>(textureNameRFR, ',');
			std::shared_ptr<Texture> textureRFR;

			if (textureListRFR.size() == 1)
			{
				std::string fullTextureNameRFR = extractPath(filename) + textureNameRFR;
				textureRFR = Texture::load(fullTextureNameRFR.c_str());
				includesTexture = true;
			}
			else if (textureListRFR.size() == 6)
			{
				std::vector<std::string> fullTextureNameRFR;
				for (int i = 0; i < 6; i++)
				{
					fullTextureNameRFR.push_back(extractPath(filename) + textureListRFR[i]);
				}
				textureRFR = Texture::load(fullTextureNameRFR[0].c_str(), fullTextureNameRFR[1].c_str(),
					fullTextureNameRFR[2].c_str(), fullTextureNameRFR[3].c_str(),
					fullTextureNameRFR[4].c_str(), fullTextureNameRFR[5].c_str());
				includesTexture = true;
			}

			std::string textureNameNM = materialNode.child("normal_texture").text().as_string();
			std::vector<std::string> textureListNM;
			textureListNM = splitString<std::string>(textureNameNM, ',');
			std::shared_ptr<Texture> textureNM;

			if (textureListNM.size() == 1)
			{
				std::string fullTextureNameNM = extractPath(filename) + textureNameNM;
				textureNM = Texture::load(fullTextureNameNM.c_str());
				includesTexture = true;
			}
			else if (textureListNM.size() == 6)
			{
				std::vector<std::string> fullTextureNameNM;
				for (int i = 0; i < 6; i++)
				{
					fullTextureNameNM.push_back(extractPath(filename) + textureListNM[i]);
				}
				textureNM = Texture::load(fullTextureNameNM[0].c_str(), fullTextureNameNM[1].c_str(),
					fullTextureNameNM[2].c_str(), fullTextureNameNM[3].c_str(),
					fullTextureNameNM[4].c_str(), fullTextureNameNM[5].c_str());
				includesTexture = true;
			}

			// Read color

			glm::vec4 color = glm::vec4(1,1,1,1);
			std::string colorString = materialNode.child("color").text().as_string();

			if (colorString != "")
			{
				std::vector<float> colorVector = splitString<float>(colorString, ',');
				color = glm::vec4(colorVector.at(0), colorVector.at(1),
					colorVector.at(2), colorVector.at(3));
			}
			// Read shininess
			std::string shininessString = materialNode.child("shininess").text().as_string();
			uint8_t shininess = 0;

			if (colorString != "")
				shininess = stoi(shininessString);

			// Read refraction coef

			std::string rfrcoefString = materialNode.child("refract_coef").text().as_string();
			float refractionCoef = 0.0f;

			if (rfrcoefString != "")
				refractionCoef = stof(rfrcoefString);

			// Read blend mode

			std::string blendmodeString = materialNode.child("blend").text().as_string();
			std::string blendmode = "alpha";

			if (blendmodeString != "")
				blendmode = blendmodeString;


			// Read culling coef

			std::string cullingString = materialNode.child("culling").text().as_string();
			bool culling = true;

			if (cullingString != "")
			{
				if (cullingString == "false")
					culling = false;
			}

			// Read depthwrite coef

			std::string depthwriteString = materialNode.child("depthwrite").text().as_string();
			bool depthwrite = true;

			if (depthwriteString != "")
			{
				if (depthwriteString == "false")
					depthwrite = false;
			}

			// Read Indices from node
			std::string indices = bufferNode.child("indices").text().as_string();
			std::vector<uint16_t> indicesVector;
			
			if (indices != "")
				indicesVector = splitString<uint16_t>(indices, ',');
			else
				return nullptr;

			// Read Coordinates from node
			std::string coords = bufferNode.child("coords").text().as_string();
			std::vector<float> coordsVector;
			if (coords != "")
				coordsVector = splitString<float>(coords, ',');
			else
				return nullptr;

			// Read Texture Coordinates from node
	
			std::vector<float> texCoordsVector;
			std::string texcoords = bufferNode.child("texcoords").text().as_string();
			if (texcoords != "")
			{
				texCoordsVector = splitString<float>(texcoords, ',');
				includescoordsTexture = true;
			}

			// Read Normals from node
			std::vector<float> normalsVector;
			std::string normalsString = bufferNode.child("normals").text().as_string();
			if (normalsString != "")
			{
				normalsVector = splitString<float>(normalsString, ',');
				includesNormals = true;

			}

			// Read Tangent from node
			std::vector<float> tangentVector;
			std::string tangentString = bufferNode.child("tangents").text().as_string();
			if (tangentString != "")
			{
				tangentVector = splitString<float>(tangentString, ',');
				includesTangent = true;

			}

			std::vector<Vertex> vertexVector;

			for (int i=0, j=0, k=0; j< coordsVector.size(); i += 2, j += 3, k+= 3)
			{
				Vertex vertex;
				if (includescoordsTexture)
				{
					vertex.texture = glm::vec2(texCoordsVector[i], texCoordsVector[i + 1]);
				}

				vertex.position = glm::vec3(coordsVector[j], coordsVector[j+1], coordsVector[j+2]);

				if (includesNormals)
				{
					vertex.normal = glm::vec3(normalsVector[k], normalsVector[k + 1], normalsVector[k + 2]);
				}
				if (includesTangent)
				{
					vertex.tangent = glm::vec3(tangentVector[k], tangentVector[k + 1], tangentVector[k + 2]);
				}
				vertexVector.push_back(vertex);
			}

			Material material = Material::Material(texture, nullptr);

			material.setShininess(shininess);
			material.setColor(color);
			material.setCulling(culling);
			material.setDepthWrite(depthwrite);

			if (disableLighting) material.setLighting(false);

			if (blendmode == "add") material.setBlendMode(Material::BlendMode::ADD);
			else if (blendmode == "alpha") material.setBlendMode(Material::BlendMode::ALPHA);
			else if (blendmode == "null") material.setBlendMode(Material::BlendMode::MUL);


			if (textureListRFL.size() == 6 || textureListRFL.size() == 1) material.setReflectionTexture(textureRFL);
			if (textureListRFR.size() == 6 || textureListRFR.size() == 1) material.setRefractionTexture(textureRFR);
			if (textureListNM.size() == 6 || textureListNM.size() == 1) material.setNormalTexture(textureNM);
			material.setRefractionCoef(refractionCoef);

			std::shared_ptr<Buffer> buffer (new Buffer(vertexVector, indicesVector));

			mesh->addBuffer(buffer, material);
		}

		return mesh;
	}
	else {
		// No se ha podido cargar
		std::cout << result.description() << std::endl;
		return nullptr;
	}
}
