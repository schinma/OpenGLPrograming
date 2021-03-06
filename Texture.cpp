#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture() : path(), type()
{
}

Texture::Texture(Type t) : path(), type(t)
{
}

Texture::~Texture()
{
}

void Texture::loadTexture(const std::string &file)
{
	path = file;
	fullPath = "img/" + path;

	std::cout << "Loading texture lololo:" + file + " at path :" + fullPath << std::endl;

	int width, height, channel;
	unsigned char *texture_img = stbi_load(fullPath.c_str(), &width, &height, &channel, 0);

	if (texture_img) {
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(2, &tex_2d[0]);
		glBindTexture(GL_TEXTURE_2D, tex_2d[0]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, texture_img);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(texture_img);
	}
	else {
		std::cout << "Texture " + file + " failed to be loaded at path: " + path << std::endl;
	}
}

void Texture::loadCubeMapTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex_2d[0]);  //set the texID as a member variable
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_2d[0]);

	const char * suffixes[] = { "left", "right", "top", "down", "back", "front" };

	GLuint targets[] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; i++) {
		int channel;
		int width, height;
		std::string filename;
		filename = "img/" + std::string(suffixes[i]) + std::string(".JPG");

		std::cout << "Loading texture :" + filename << std::endl;

		unsigned char * image = stbi_load(filename.c_str(), &width, &height, &channel, 0);
		glTexImage2D(targets[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		if (!image) {
			std::cout << "Texture " + filename + " failed to be loaded" << std::endl;
		}
		stbi_image_free(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture::loadTexture(const std::string &file, const std::string &file2)
{

		int width, height, channel;
		std::string path = "img/" + file;
		unsigned char *texture_img = stbi_load(path.c_str(), &width, &height, &channel, 0);

		std::cout << "Loading texture :" + file + " at path :" + path << std::endl;

		if (texture_img) {
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(2, tex_2d);

			glBindTexture(GL_TEXTURE_2D, tex_2d[0]);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, texture_img);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(texture_img);

		} else {
			std::cout << "Texture " + file + " failed to be loaded at path: " + path << std::endl;
		}


		path = "img/" + file2;
		texture_img = stbi_load(path.c_str(), &width, &height, &channel, 0);

		std::cout << "Loading texture :" + file + " at path :" + path << std::endl;

		if (texture_img) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex_2d[1]);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, texture_img);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(texture_img);

		} else {
			std::cout << "Texture " + file + " failed to be loaded at path: " + path << std::endl;
		}
}

GLuint Texture::getIdNumber(int i) const
{
	return (tex_2d[i]);
}

Texture::Type Texture::getType() const
{
	return (type);
}

const std::string Texture::getPath() const
{
	return (path);
}