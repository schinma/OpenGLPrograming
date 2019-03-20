#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"
#include <string>
#include <vector>
#include "Material.h"

class Texture
{
public:
	Texture();
	~Texture();

	void loadTexture(const std::string &file);
	void loadTexture(const std::string &file, const std::string &file2);
	GLuint getIdNumber(int i) const;

private:

	GLuint tex_2d[2];
	unsigned char *texture_img;
	int width, height, channel;

	Material _material;

};

#endif /* TEXTURE_H */