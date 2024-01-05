
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "GUClock.h"
#include "PrincipleAxes.h"
#include "AIMesh.h"


using namespace std;
using namespace glm;


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

//Window height and width
const unsigned int initWidth = 512;
const unsigned int initHeight = 512;

#pragma endregion

// Function prototypes
void renderScene();
void updateScene();
void resizeWindow(GLFWwindow* window, int width, int height);
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);


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

	buildingPhase2Mesh = new AIMesh(string("Assets\\Building\\BuildingPhase1.obj"));//imports MarsGroundFlat mesh
	if (buildingPhase2Mesh) {
		buildingPhase2Mesh->addTexture(string("Assets\\Textures\\BuildingPhase1.bmp"), FIF_BMP);//imports texture if MarsGroundFlat mesh is imported
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

	if (buildingPhase2Mesh)
	{

		mat4 groundFlatTranslate = translate(identity<mat4>(), vec3(0.0f, 10.0f, 0.0f));
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
}


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

#pragma endregion