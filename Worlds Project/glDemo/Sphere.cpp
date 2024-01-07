#include "Sphere.h"
#include "TextureLoader.h"
#include "shader_setup.h"
#include "core.h"

using namespace std;
using namespace glm;

Sphere::Sphere(std::string filename, GLuint meshIndex) : AIMesh(filename, meshIndex)
{
	sphereTexture = loadTexture("Assets\\Textures\\blue.png", FIF_PNG);

	shader = setupShaders(string("Assets\\Shaders\\basic_shader.vert"), string("Assets\\Shaders\\basic_shader.frag"));

	shader_mvpMatrix = glGetUniformLocation(shader, "mvpMatrix");
	shader_sphereTexture = glGetUniformLocation(shader, "sphereTexutre");

	glUseProgram(shader);
	glUniform1i(shader_sphereTexture, 1);
	glUseProgram(0);
}

void Sphere::preRender()
{
	AIMesh::preRender();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sphereTexture);

	glActiveTexture(GL_TEXTURE0);
}

void Sphere::render(mat4 transform)
{
	glUseProgram(shader);
	glUniformMatrix4fv(shader_mvpMatrix, 1, GL_FALSE, (GLfloat*)&transform);
	AIMesh::render();
}

void Sphere::postRender()
{
	AIMesh::postRender();
}
