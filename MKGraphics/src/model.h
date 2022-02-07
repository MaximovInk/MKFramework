#pragma once

#include "string"
#include "vector"

#include "mesh.h"
//#include "texture.h"
#include "camera.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "API.H"

class MKEngine::mesh;

class MK_ENGINE_API model {
public:
	model(const char* file) {
		loadModel(file);
	}

	void draw();
	//void drawM(shader& shader, glm::mat4 camMatrix);

private:
	std::vector< MKEngine::mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	MKEngine::mesh processMesh(aiMesh* mesh, const aiScene* scene);
	//std::vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};