#include <hf/render/geometry/model.h>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>

//Assimp includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

namespace hf::render
{

	Model::Model(std::filesystem::path path)
	{
		loadModel(path);
	}

	void Model::draw(Shader& shader)
	{
		for (Mesh& mesh : m_Meshes)
		{
			mesh.draw(shader);
		}
	}

	void Model::loadModel(std::filesystem::path path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.string(),
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "Failed to load model: " << path.string() << " " << importer.GetErrorString() << std::endl;
			return;
		}
		m_ParentDirectory = path.parent_path();

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		//Process all the meshes in the node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}
		//then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> hfVertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<Texture>> hfTextures;

		//Process vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex hfVertex;

			//Position
			glm::vec3 positionVector;
			positionVector.x = mesh->mVertices[i].x;
			positionVector.y = mesh->mVertices[i].y;
			positionVector.z = mesh->mVertices[i].z;
			hfVertex.Position = positionVector;

			//Normal
			glm::vec3 normalVector;
			normalVector.x = mesh->mNormals[i].x;
			normalVector.y = mesh->mNormals[i].y;
			normalVector.z = mesh->mNormals[i].z;
			hfVertex.Normal = normalVector;

			//Texture coordinates
			//The mesh does not always contain texture coordinates
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 texCoordsVector;
				texCoordsVector.x = mesh->mTextureCoords[0][i].x;
				texCoordsVector.y = mesh->mTextureCoords[0][i].y;
				hfVertex.TexCoords = texCoordsVector;
			}

			hfVertices.push_back(hfVertex);
		}

		//Process indices (iterate over the faces and get the indices)
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//Process textures
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			//Diffuse
			std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
			hfTextures.insert(hfTextures.end(), std::make_move_iterator(diffuseMaps.begin()), std::make_move_iterator(diffuseMaps.end()));
			//Specular
			std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
			hfTextures.insert(hfTextures.end(), std::make_move_iterator(specularMaps.begin()), std::make_move_iterator(specularMaps.end()));
		}


		return Mesh(std::move(hfVertices), std::move(indices), std::move(hfTextures));
	}

	std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType)
	{
		std::vector<std::shared_ptr<Texture>> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString fileName;
			mat->GetTexture(type, i, &fileName);
			//Check if the texture is already loaded, if so, continue to next iteration: skip loading a new texture
			const auto it = m_TexturesLoaded.find(fileName.C_Str());
			if (it != m_TexturesLoaded.end())
			{
				textures.push_back(it->second);
				continue;
			}
		
			auto texture = std::make_shared<Texture>(std::string{ fileName.C_Str() }, m_ParentDirectory.string(), textureType);
			m_TexturesLoaded.emplace(fileName.C_Str(), texture);
			textures.push_back(std::move(texture));
		}
		return textures;
	}

}