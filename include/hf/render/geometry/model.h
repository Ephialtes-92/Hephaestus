#pragma once

#include <filesystem>
#include <assimp/material.h>
#include <hf/render/shaders/shader.h>
#include <hf/render/geometry/mesh.h>
#include <hf/render/textures/texture.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

//just fowrard declare the assimp classes we need to avoid including the entire library in the header
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace hf::render
{

	class Model {
	public:
		Model(std::filesystem::path path);
		void draw(Shader& shader);

	private:
		void loadModel(std::filesystem::path path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);

		//Model data
		std::vector<Mesh> m_Meshes;

		std::filesystem::path m_ParentDirectory;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_TexturesLoaded;

	};

}