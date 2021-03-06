#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <vector>

#include "assimp\scene.h"
#include "assimp\mesh.h"
#include "Loader.h"
#include "IObject.h"
#include "Texture.h"


class Mesh : public IObject
{
public:
	struct MeshEntry {
		static enum BUFFERS {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, TANGENT_BUFFER, BITTANGENT_BUFFER
		};
		GLuint vao;
		GLuint vbo[6];


		unsigned int elementCount;
		aiColor3D dcolor;
		aiColor3D acolor;
		aiColor3D scolor;
		float shininessStrength;
		std::vector<Texture> textures;
		MeshEntry(aiMesh *mesh, const aiScene* scene, Mesh * m);
		~MeshEntry();
		Mesh * parent;
		void render(ShaderProgram *shader);
		void loadTextures(const aiMaterial *mat, aiTextureType type, Texture::Type texType, Mesh *m);
		};
		
public:
	Mesh(const char *filename, ShaderProgram * sh);
	~Mesh(void);

	
	std::vector<MeshEntry*> meshEntries;
	std::vector<Texture> texturesLoaded;
	ShaderProgram * shader;
	void draw();
	std::vector<Texture> loadTextures(const aiMaterial *mat, aiTextureType type, Texture::Type texType);
};