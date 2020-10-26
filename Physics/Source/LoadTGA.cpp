#include <iostream>
#include <fstream>
#include <GL\glew.h>
#include "LoadTGA.h"

GLuint LoadTGA(const char* filePath){ //Load TGA file to memory
	std::ifstream fileStream(filePath, std::ios::binary);
	if(!fileStream.is_open()){
		std::cout << "Unable to open \"" << filePath << "\".\n";
		return 0;
	}
	GLubyte header[18]; //1st 6 useful header bytes
	GLuint bytesPerPixel; //No. of bytes per pixel in TGA file
	GLuint imageSize; //For setting memory
	GLubyte* data;
	GLuint texture = 0;
	unsigned width, height;
	fileStream.read((char*)header, 18);
	width = header[12] + header[13] * 256;
	height = header[14] + header[15] * 256;
	if(width <= 0 || height <= 0 || (header[16] != 24 && header[16] != 32)){ //If... || !(TGA is 24 bits or TGA is 32 bits)
		fileStream.close(); //Close file on failure
		std::cout << "File header error.\n";
		return 0;
	}
	bytesPerPixel = header[16] / 8;
	imageSize = width * height * bytesPerPixel;	//Calc mem needed for TGA data
	data = new GLubyte[imageSize];
	fileStream.seekg(18, std::ios::beg);
	fileStream.read((char*)data, imageSize);
	fileStream.close();
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if(bytesPerPixel == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	} else{ //bytesPerPixel == 4
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); //Can change
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR); //Can change
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); //Can change
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); //Can change
	glGenerateMipmap(GL_TEXTURE_2D);
	float maxAnisotropy = 1.f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GLint(maxAnisotropy));
	delete[] data;
	return texture;
}