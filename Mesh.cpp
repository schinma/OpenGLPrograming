
#include <vector>
#include <iostream>
#include "Mesh.h"

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/SOIL.h>

/**
*	Constructor, loading the specified aiMesh
**/
Mesh::MeshEntry::MeshEntry(aiMesh *mesh, const aiScene* scene, Mesh * m)
{
	parent = m;

	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;
	vbo[TANGENT_BUFFER] = NULL;
	vbo[BITTANGENT_BUFFER] = NULL;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	elementCount = mesh->mNumFaces * 3;

	if (mesh->HasPositions()) {
		float *vertices = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		delete[] vertices;
	}


	if (mesh->HasTextureCoords(0)) {
		float *texCoords = new float[mesh->mNumVertices * 2];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		delete[] texCoords;
	}


	if (mesh->HasNormals()) {
		float *normals = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1, &vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		delete[] normals;
	}

	if (mesh->HasTangentsAndBitangents()) {

		float *tangent = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			tangent[i * 3] = mesh->mTangents[i].x;
			tangent[i * 3 + 1] = mesh->mTangents[i].y;
			tangent[i * 3 + 2] = mesh->mTangents[i].z;
		}
		float *bittangent = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			bittangent[i * 3] = mesh->mBitangents[i].x;
			bittangent[i * 3 + 1] = mesh->mBitangents[i].y;
			bittangent[i * 3 + 2] = mesh->mBitangents[i].z;
		}

		glGenBuffers(1, &vbo[TANGENT_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[TANGENT_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLuint), tangent, GL_STATIC_DRAW);

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(4);

		glGenBuffers(1, &vbo[BITTANGENT_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[BITTANGENT_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLuint), bittangent, GL_STATIC_DRAW);

		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(5);

		delete[] tangent;
		delete[] bittangent;
	}

	if (mesh->HasFaces()) {
		unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
		for (int i = 0; i < mesh->mNumFaces; ++i) {
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		delete[] indices;
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		material->Get(AI_MATKEY_SHININESS, shininessStrength);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, dcolor);
		material->Get(AI_MATKEY_COLOR_AMBIENT, acolor);
		material->Get(AI_MATKEY_COLOR_SPECULAR, scolor);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* Loading the different texture types */
	const aiMaterial *pMaterial = scene->mMaterials[mesh->mMaterialIndex];

	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		loadTextures(pMaterial, aiTextureType_DIFFUSE, Texture::DIFFUSE, m);
	}
	/*if (pMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0) {
		loadTextures(pMaterial, aiTextureType_HEIGHT, Texture::NORMAL, m);
	}*/
}


/*
*	Load the textures 
*/
void Mesh::MeshEntry::loadTextures(const aiMaterial *mat, aiTextureType type, Texture::Type texType, Mesh *m)
{
	aiString path;

	if (mat->GetTexture(type, 0, &path,
		NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
		std::string fullPath = path.data;
		bool load = true;
		for (int i = 0; i < m->texturesLoaded.size(); i++) {
			if (m->texturesLoaded[i].getPath() == fullPath) {
				textures.push_back(m->texturesLoaded[i]);
				load = false;
				break;
			}
		}
		if (load) {
			Texture texture(texType);
			texture.loadTexture(fullPath);
			textures.push_back(texture);
			m->texturesLoaded.push_back(texture);
		}
	}
	else if (type == aiTextureType_NORMALS) {
		std::cout << "bleh\n";
	}
}

/**
*	Deletes the allocated OpenGL buffers
**/
Mesh::MeshEntry::~MeshEntry() {
	if (vbo[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
	}

	if (vbo[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
	}

	if (vbo[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
	}

	if (vbo[INDEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &vao);
	
}

/**
*	Renders this MeshEntry
**/
void Mesh::MeshEntry::render(ShaderProgram *shader) {

	for (int j = 0; j < textures.size(); j++) {

		std::string type;
		if (textures[j].getType() == Texture::Type::DIFFUSE) {
			type = "ColorTex";
		}
		else if (textures[j].getType() == Texture::Type::NORMAL) {
			type = "NormalMapTex";
		}

		glActiveTexture(GL_TEXTURE0 + j);
		glUniform1i(shader->uniform(type), j);
		glBindTexture(GL_TEXTURE_2D, textures[j].getIdNumber(0));

	}

	glBindVertexArray(vao);
		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size/ sizeof(unsigned int), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
*	Mesh constructor, loads the specified filename if supported by Assimp
**/
Mesh::Mesh(const char *filename, ShaderProgram * sh)
{
	shader = sh;

	std::string fullname;
	fullname = std::string("./Models/")+ std::string(filename);

	Assimp::Importer importer;   //aiProcessPreset_TargetRealtime_Fast
	const aiScene* scene = importer.ReadFile(fullname.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace);

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	for (int i = 0; i < scene->mNumMeshes; ++i) {
		aiMesh *mesh = scene->mMeshes[i];

		meshEntries.push_back(new Mesh::MeshEntry(mesh, scene, this));
	}	
}

/**
*	Clears all loaded MeshEntries
**/
Mesh::~Mesh(void)
{
	for (int i = 0; i < meshEntries.size(); ++i) {

		delete meshEntries.at(i);
	}
	meshEntries.clear();
}

/**
*	Renders all loaded MeshEntries
**/
void Mesh::draw() {

	shader->use();

	for (int i = 0; i < meshEntries.size(); ++i) {

		MeshEntry * m = meshEntries[i];
		

		glm::vec3 diffuse = glm::vec3(meshEntries.at(i)->dcolor.r, meshEntries.at(i)->dcolor.g, meshEntries.at(i)->dcolor.b);
		glm::vec3 specular = glm::vec3(meshEntries.at(i)->scolor.r, meshEntries.at(i)->scolor.g, meshEntries.at(i)->scolor.b);
		glm::vec3 ambient = glm::vec3(meshEntries.at(i)->acolor.r, meshEntries.at(i)->acolor.g, meshEntries.at(i)->acolor.b);

		if (glm::length(ambient) == 0) {
			ambient = glm::vec3(0.3, 0.3, 0.3);
		}

		if (glm::length(diffuse) == 0) {
			diffuse = glm::vec3(0.8, 0.8, 0.8);
		}

		if (glm::length(specular) == 0) {
			specular = glm::vec3(0.3, 0.3, 0.3);
		}

		float shiness = meshEntries.at(i)->shininessStrength;

		if (shiness == 0)
			shiness = 10.0f;

		glUniform3fv(shader->uniform("Kd"), 1, glm::value_ptr(diffuse));
		glUniform3fv(shader->uniform("Ka"), 1, glm::value_ptr(ambient));
		glUniform3fv(shader->uniform("Ks"), 1, glm::value_ptr(specular));
		glUniform1f(shader->uniform("shininess"), shiness);

		meshEntries.at(i)->render(shader);
	}
	shader->disable();
}
