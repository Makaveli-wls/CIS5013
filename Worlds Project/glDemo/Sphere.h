#pragma once

#include "AIMesh.h"

class Sphere : public AIMesh {

	GLuint sphereTexture = 0;

	GLuint shader = 0;
	GLint shader_mvpMatrix = 1;
	GLint shader_sphereTexture = 1;

public:
	Sphere(std::string filename, GLuint meshIndex = 0);

	void preRender();
	void render(glm::mat4 transform);
	void postRender();


};