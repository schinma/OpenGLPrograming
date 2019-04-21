#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"
#include <string>
#include <vector>
#include <iostream>
#include "Material.h"

class Texture
{
public:

	enum Type {
		DIFFUSE, NORMAL
	};

	Texture();
	Texture(Type type);
	~Texture();

	void loadTexture(const std::string &file);
	void loadTexture(const std::string &file, const std::string &file2);
	void loadCubeMapTexture();
	GLuint getIdNumber(int i) const;
	Texture::Type getType() const;
	const std::string getPath() const;

private:

	GLuint tex_2d[2];
	std::string path;
	std::string fullPath;
	Type type;

};

#endif /* TEXTURE_H */