#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Window.h"
#include "MatrixTransform.h"
#include "Geode.h"
#include "Light.h"
#include "Skybox.h"
#include "Shader.h"
#include "Parser.h"
#include "ParticleEmitter.h"
#include <Soil/SOIL.h>

namespace
{
	int windowWidth = 512, windowHeight = 512;
	const double zNear = 0.1, zFar = 1000.0, fov = 60.0;

	double frame = 0.0, frameElapsed = 0, lastFrameTime = 0.0;

	bool fullscreen = false;

	auto scene = std::make_shared<MatrixTransform>();
	auto camera = std::make_shared<MatrixTransform>();
	auto sphere = std::make_shared<Sphere>();
	auto cone = std::make_shared<Cone>();
	auto light = std::make_shared<Light>(GL_LIGHT0);
	auto skybox = std::make_shared<Skybox>();
	auto tailEmitter = std::make_shared<ParticleEmitter>(1000);
	auto ship = parseModel("models/arc170lp.obj");
	auto shipTransform = std::make_shared<MatrixTransform>();

	Vector3 center(0, 0, 1.5);
	Vector3 eye(0, 0, 0);
	Vector3 up(0, 1, 0);
	
	bool useShader = false;
	bool rocketsOn = false;

	GLuint program;
}

double aspect()
{
	if (windowHeight == 0) {
		return 1.0;
	}
	return static_cast<double>(windowWidth) / windowHeight;
}

void motionCallback(int x, int y)
{
}

void mouseCallback(int button, int state, int x, int y)
{
}

void reshapeCallback(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, static_cast<double>(windowWidth) / windowHeight, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboardCallback(unsigned char key, int, int)
{
	const double dr = 2.0, ds = 0.1, dt = 0.1;
	Vector3 forward = (eye - center).normalize();
	switch (key) {
	case 'z':
		camera->transform *= Transform::rotateX(dr);
		break;
	case 'Z':
		camera->transform *= Transform::rotateX(-dr);
		break;
	case 'x':
		camera->transform *= Transform::rotateY(dr);
		break;
	case 'X':
		camera->transform *= Transform::rotateY(-dr);
		break;
	case 's':
		camera->transform *= Transform::translate(-forward);
		break;
	case 'S':
		camera->transform *= Transform::translate(forward);
		break;
	case 'w':
		shipTransform->transform *= Transform::rotateX(1.0);
		rocketsOn = !rocketsOn;
		/*
		if (rocketsOn) {
			tailEmitter->enabled = true;
		} else {
			tailEmitter->enabled = false;
		}
		*/
		break;
	case 'e':
		useShader = !useShader;
		if (useShader) {
			glUseProgram(program);
		} else {
			glUseProgram(0);
		}
		break;
	case 8:	// backspace
		scene->transform = Matrix4().identity();
		tailEmitter->reset();
		break;
	case 13:	// enter
		if (fullscreen) {
			exitFullScreen();
		} else {
			fullscreen = true;
			glutFullScreen();
		}
		break;
	case 27:	// escape
		if (fullscreen) {
			exitFullScreen();
		} else {
			exit(0);
		}
	}
	displayCallback();
}

void exitFullScreen()
{
	fullscreen = false;
	glutReshapeWindow(512, 512);
	glutPositionWindow(100, 100);
}

void specialCallback(int key, int, int)
{
	switch (key) {
	case GLUT_KEY_F1:
		break;
	default:
		break;
	}
}

void displayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera->render(Matrix4().identity());
	glutSwapBuffers();
}

void idleCallback()
{
	++frameElapsed;
	auto time = glutGet(GLUT_ELAPSED_TIME);
	if (time - lastFrameTime > 1000) {
		std::cout << "FPS: " << frameElapsed * 1000 / (time - lastFrameTime) << '\n';
		lastFrameTime = time;
		frameElapsed = 0;
	}
	displayCallback();
}

void shaderInit()
{
	glewInit();
	if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader) {
		exit(1);
	}
	program = glCreateProgram();
	/*
	glAttachShader(program, loadShader(GL_VERTEX_SHADER, "shaders/water.vert"));
	glAttachShader(program, loadShader(GL_FRAGMENT_SHADER, "shaders/water.frag"));
	*/
	glLinkProgram(program);

	program = glCreateProgram();
	glAttachShader(program, loadShader(GL_VERTEX_SHADER, "shaders/skybox.vert"));
	glAttachShader(program, loadShader(GL_FRAGMENT_SHADER, "shaders/skybox.frag"));
	glLinkProgram(program);
	skybox->programId = program;
}

int main(int argc, char** argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("IronMan");

	shaderInit();

	glutMotionFunc(motionCallback);
	glutMouseFunc(mouseCallback);
	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(displayCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);
	glutIdleFunc(idleCallback);

	initScene();

	glutMainLoop();
}

void loadTextures()
{
	GLuint shipTexture = SOIL_load_OGL_texture
		(
		"assets/arc170.tga",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	if (!shipTexture) {
		std::cout << "SOIL loading error: " << SOIL_last_result() << '\n';
	}
	ship->textureId = shipTexture;

	GLuint fireTexture = SOIL_load_OGL_texture
		(
		"assets/fire2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	if (!fireTexture) {
		std::cout << "SOIL loading error: " << SOIL_last_result() << '\n';
	}
	tailEmitter->textureId = fireTexture;

	std::vector<std::string> skyboxTextures
	{
		"assets/spacel.png",
		"assets/spaceb.png",
		"assets/spacer.png",
		"assets/spaceba.png",
		"assets/spacef.png",
		"assets/spacet.png"
	};
	skybox->loadTextures(skyboxTextures);
}

void initScene()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	camera->transform = Transform::lookAt(center, eye, up);
	camera->transform *= Transform::rotateX(15.0);

	skybox->transform = Transform::scale(100);
	shipTransform->transform *= Transform::translate({ 0.0, 0.0, 0.0 });

	loadTextures();
	tailEmitter->particleRadius = 3.0;
	tailEmitter->transform = Transform::scale(0.05) * Transform::rotateX(90.0) * Transform::translate({ 0.0, 6.0, 0.0 });
	tailEmitter->emitRate = 20;
	tailEmitter->enabled = true;

	camera->attach(scene);
	scene->attach(skybox);
	scene->attach(light);
	scene->attach(shipTransform); 
	shipTransform->attach(ship);
	shipTransform->attach(tailEmitter);
}