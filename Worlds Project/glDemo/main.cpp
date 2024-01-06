
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "GUClock.h"
#include "PrincipleAxes.h"
#include "AIMesh.h"


using namespace std;
using namespace glm;

/*struct directionalLight {

	vec3 direction;
	vec3 colour;

	directionalLight() {

		direction = vec3(0.0f, -1.0f, 0.0f); 
		colour = vec3(0.0f, 1.0f, 0.0f);
	}

	directionalLight(vec3 direction, vec3 colour = vec3(0.0f, 1.0f, 0.0f)) {

		this->direction = direction;
		this->colour = colour;
	}
};*/

#pragma region Global variables

//Game clock
GUClock* gamesClock = nullptr;

//Main camera
ArcballCamera* Camera = nullptr;

//Scene objects
CGPrincipleAxes* principleAxes = nullptr;
AIMesh* npcMesh = nullptr;
AIMesh* groundFlatMesh = nullptr;
AIMesh* buildingPhase2Mesh = nullptr;
AIMesh* buildingPhase1Mesh = nullptr;

//Window height and width
const unsigned int initWidth = 512;
const unsigned int initHeight = 512;

/*
GLuint				texDirLightShader;
GLint				texDirLightShader_modelMatrix;
GLint				texDirLightShader_viewMatrix;
GLint				texDirLightShader_projMatrix;
GLint				texDirLightShader_texture;
GLint				texDirLightShader_lightDirection;
GLint				texDirLightShader_lightColour;

float directLightTheta = 0.0f;
directionalLight directLight = directionalLight(vec3(cosf(directLightTheta), sinf(directLightTheta), 0.0f));
*/
#pragma endregion

// Function prototypes
void renderScene();
void updateScene();
void resizeWindow(GLFWwindow* window, int width, int height);
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void zoomIn(GLFWwindow* window);
void zoomOut(GLFWwindow* window);
void downLayer(GLFWwindow* window);
void upLayer(GLFWwindow* window);
void rotateLeft(GLFWwindow* window);
void rotateRight(GLFWwindow* window);
//void renderWithDirectionalLight();

bool downLayerEnabled = false;
float rotationsLeft = 0.0f;
float rotationsRight = 0.0f;



int main() 
{
	//Initialisation

	gamesClock = new GUClock();

#pragma region OpenGL and window setup

	
	glfwInit();// Initialise glfw 
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);//window setup
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "Worlds - CIS5013", NULL, NULL);

	//Validate if window has been opened correctly
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	

	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, resizeWindow); // resize window callback
	glfwSetKeyCallback(window, keyboardHandler); // Keyboard input callback

	glewInit();// Initialise glew
	resizeWindow(window, initWidth, initHeight);//Setup windows size

#pragma endregion


	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black
	glClearDepth(1.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	
	//Textures and VBOs
	
	Camera = new ArcballCamera(-30.0f, -50.0f, 300.0f, 55.0f, 1.0f, 0.1f, 1000000.0f);
	principleAxes = new CGPrincipleAxes();

	npcMesh = new AIMesh(string("Assets\\NPC\\npc.obj"));//imports NPC model mesh
	if (npcMesh) {
		npcMesh->addTexture(string("Assets\\Textures\\BuildingPhase1.bmp\\"), FIF_BMP);//imports texture of npcMesh if npcMesh is imported
	}

	groundFlatMesh = new AIMesh(string("Assets\\Ground\\MarsGroundFlat.obj"));//imports MarsGroundFlat mesh
	if (groundFlatMesh) {
		groundFlatMesh->addTexture(string("Assets\\Textures\\MarsGroundTexture.jpg"), FIF_JPEG);//imports texture if MarsGroundFlat mesh is imported
	}

	buildingPhase1Mesh = new AIMesh(string("Assets\\Building\\BuildingPhase1.obj"));//imports MarsGroundFlat mesh
	if (buildingPhase1Mesh) {
		buildingPhase1Mesh->addTexture(string("Assets\\Textures\\BuildingPhase1.bmp"), FIF_BMP);//imports texture if MarsGroundFlat mesh is imported
	}

	buildingPhase2Mesh = new AIMesh(string("Assets\\Building\\BuildingPhase2.obj"));//imports MarsGroundFlat mesh
	if (buildingPhase2Mesh) {
		buildingPhase2Mesh->addTexture(string("Assets\\Textures\\BuildingPhase2.bmp"), FIF_BMP);//imports texture if MarsGroundFlat mesh is imported
	}


	//Main loop

	while (!glfwWindowShouldClose(window))
	{
		updateScene();						//update scene for animations
		renderScene();						//Calls render scene function to render scene
		glfwSwapBuffers(window);			//Displays rendered scene
		glfwPollEvents();
	}

	glfwTerminate();

	if (gamesClock) 
	{
		gamesClock->stop();
		gamesClock->reportTimingData();
	}

	return 0;
}

//rendering the scene
void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 cameraTransform = Camera->projectionTransform() * Camera->viewTransform();

#if 1
	
	if (npcMesh) 
	{

		// Setup transforms
		mat4 npcTranslate = translate(identity<mat4>(), vec3(40.0f, -11.0f, 0.0f));
		mat4 T = cameraTransform * npcTranslate;
		glLoadMatrixf((GLfloat*)&T);

		npcMesh->preRender();
		npcMesh->render();
		npcMesh->postRender();
	}
	
	if (groundFlatMesh) 
	{

		// Setup transforms
		mat4 groundFlatTranslate = translate(identity<mat4>(), vec3(0.0f, -10.0f, 12.5f));
		mat4 T = cameraTransform * groundFlatTranslate;
		glLoadMatrixf((GLfloat*)&T);

		groundFlatMesh->preRender();
		groundFlatMesh->render();
		groundFlatMesh->postRender();
	}
	if (groundFlatMesh) 
	{

		mat4 groundFlatTranslate = translate(identity<mat4>(), vec3(0.0f, -10.0f, -112.5f));
		mat4 T = cameraTransform * groundFlatTranslate;
		glLoadMatrixf((GLfloat*)&T);

		groundFlatMesh->preRender();
		groundFlatMesh->render();
		groundFlatMesh->postRender();

	}

	if (groundFlatMesh) 
	{

		mat4 groundFlatTranslate = translate(identity<mat4>(), vec3(125.0f, -10.0f, -112.5f));
		mat4 T = cameraTransform * groundFlatTranslate;
		glLoadMatrixf((GLfloat*)&T);

		groundFlatMesh->preRender();
		groundFlatMesh->render();
		groundFlatMesh->postRender();

	}

	if (groundFlatMesh) 
	{

		mat4 groundFlatTranslate = translate(identity<mat4>(), vec3(125.0f, -10.0f, 12.5f));
		mat4 T = cameraTransform * groundFlatTranslate;
		glLoadMatrixf((GLfloat*)&T);

		groundFlatMesh->preRender();
		groundFlatMesh->render();
		groundFlatMesh->postRender();

	}

	if (buildingPhase1Mesh)
	{

		mat4 groundFlatTranslate = translate(identity<mat4>(), vec3(40.0f, 20.0f, -15.0f));
		mat4 T = cameraTransform * groundFlatTranslate;
		glLoadMatrixf((GLfloat*)&T);

		buildingPhase1Mesh->preRender();
		buildingPhase1Mesh->render();
		buildingPhase1Mesh->postRender();

	}

	if (buildingPhase2Mesh)
	{

		mat4 groundFlatTranslate = translate(identity<mat4>(), vec3(100.0f, 10.0f, -50.0f));
		mat4 T = cameraTransform * groundFlatTranslate;
		glLoadMatrixf((GLfloat*)&T);

		buildingPhase2Mesh->preRender();
		buildingPhase2Mesh->render();
		buildingPhase2Mesh->postRender();

	}
#endif*/

}


//For animation update
void updateScene() 
{
	float tDelta = 0.0f;

	if (gamesClock)
	{

		gamesClock->tick();
		tDelta = (float)gamesClock->gameTimeDelta();
	}

	//renderWithDirectionalLight();
}

//Lighting code that didnt work
/*void renderWithDirectionalLight() {

	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get camera matrices
	mat4 cameraProjection = Camera->projectionTransform();
	mat4 cameraView = Camera->viewTransform();

	// Plug-in texture-directional light shader and setup relevant uniform variables
	// (keep this shader for all textured objects affected by the light source)
	glUseProgram(texDirLightShader);

	glUniformMatrix4fv(texDirLightShader_viewMatrix, 1, GL_FALSE, (GLfloat*)&cameraView);
	glUniformMatrix4fv(texDirLightShader_projMatrix, 1, GL_FALSE, (GLfloat*)&cameraProjection);
	glUniform1i(texDirLightShader_texture, 0); // set to point to texture unit 0 for AIMeshes
	glUniform3fv(texDirLightShader_lightDirection, 1, (GLfloat*)&(directLight.direction));
	glUniform3fv(texDirLightShader_lightColour, 1, (GLfloat*)&(directLight.colour));



	if (npcMesh) {

		mat4 modelTransform = glm::scale(identity<mat4>(), vec3(10.0f, 1.0f, 10.0f));

		glUniformMatrix4fv(texDirLightShader_modelMatrix, 1, GL_FALSE, (GLfloat*)&modelTransform);

		groundMesh->preRender();
		groundMesh->render();
		groundMesh->postRender();
	}


	//
	// For demo purposes, render directional light source
	//

	// Restore fixed-function pipeline
	glUseProgram(0);

	mat4 cameraT = cameraProjection * cameraView;
	glLoadMatrixf((GLfloat*)&cameraT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glColor3f(directLight.colour.r, directLight.colour.g, directLight.colour.b);
	glVertex3f(directLight.direction.x * 10.0f, directLight.direction.y * 10.0f, directLight.direction.z * 10.0f);
	glEnd();
}*/

#pragma region Event handler functions

//Window resize
void resizeWindow(GLFWwindow* window, int width, int height)
{
	if (Camera) {

		Camera->setAspect((float)width / (float)height);
	}

	glViewport(0, 0, width, height);// Draw view window
}


// Function for kayboard key handling
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) 
	{

		// check what key is pressed
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break;

			case GLFW_KEY_W:
				zoomIn(window);
				break;

			case GLFW_KEY_S:
				zoomOut(window);
				break;

			case GLFW_KEY_Q:
				downLayer(window);
				break;
			case GLFW_KEY_E:
				upLayer(window);
				break;
			case GLFW_KEY_A:
				rotateLeft(window);
				break;
			case GLFW_KEY_D:
				rotateRight(window);
				break;
			default:
			{
			}
		}
	}
	else if (action == GLFW_RELEASE)
	{
		// handle key release events
		switch (key)
		{
			default:
			{
			}
		}
	}
}

void zoomIn(GLFWwindow* window)
{
	if (Camera)
	{
		Camera->scaleRadius(0.5f);
	}
}

void zoomOut(GLFWwindow* window)
{
	if (Camera)
	{
		Camera->scaleRadius(2.0f);
	}
}

void downLayer(GLFWwindow* window)
{
	glm::mat4 down(2.0f);

	if (downLayerEnabled == false)
	{
		if (Camera)
		{
			Camera->projectionMatrix += down;
			downLayerEnabled = true;
		}
	}
}

void upLayer(GLFWwindow* window)
{
	glm::mat4 up(-2.0f);
	if (downLayerEnabled == true)
	{
		if (Camera)
		{
			Camera->projectionMatrix += up;
			downLayerEnabled = false;
		}
	}
}

void rotateLeft(GLFWwindow* window)
{
	if (rotationsLeft < 3)
	{
		if (Camera)
		{
			Camera->phi -= 15.0f;
			rotationsLeft++;
			rotationsRight--;
		}
	}
}

void rotateRight(GLFWwindow* window)
{
	if (rotationsRight < 3)
	{
		if (Camera)
		{
			Camera->phi += 15.0f;
			rotationsLeft--;
			rotationsRight++;
		}

		//directLightTheta += glm::radians(45.0f) * tDelta;
		//directLight.direction = vec3(cosf(directLightTheta), sinf(directLightTheta), 0.0f);
	}
}
#pragma endregion