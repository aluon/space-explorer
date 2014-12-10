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

namespace
{
	int windowWidth = 512, windowHeight = 512;
	const double zNear = 1.0, zFar = 1000.0, fov = 60.0;

	double frame = 0.0, frameElapsed = 0, lastFrameTime = 0.0, time;
	long frameCount;

	auto scene = std::make_shared<MatrixTransform>();
	auto camera = std::make_shared<MatrixTransform>();
	auto sphere = std::make_shared<Sphere>();
	auto cone = std::make_shared<Cone>();
	auto light = std::make_shared<Light>(GL_LIGHT0);
	auto skybox = std::make_shared<Skybox>();
	auto suit = parseModel("models/IronMan.obj");
	
	bool useShader = false;

	GLuint program;

	std::vector<std::string> skyboxTextures
	{
		"assets/PalldioPalace_intern_left.jpg",
		"assets/PalldioPalace_intern_base.jpg",
		"assets/PalldioPalace_intern_right.jpg",
		"assets/PalldioPalace_intern_back.jpg",
		"assets/PalldioPalace_intern_front.jpg",
		"assets/PalldioPalace_intern_top.jpg",
	};
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

double angle = 0.0;
void keyboardCallback(unsigned char key, int, int)
{
	const double dr = 2.0, ds = 0.1, dt = 0.1;
	switch (key) {
	case 'w':
		scene->transform *= Transform::rotateX(dr);
		break;
	case 'W':
		scene->transform *= Transform::rotateX(-dr);
		break;
	case 'q':
		scene->transform *= Transform::rotateY(dr);
		break;
	case 'Q':
		scene->transform *= Transform::rotateY(-dr);
		break;
	case 's':
		scene->transform *= Transform::scale(1.0 + ds);
		break;
	case 'S':
		scene->transform *= Transform::scale(1.0 - ds);
		break;
	case 'X':
		scene->transform *= Transform::translate({ -dt, 0.0, 0.0 });
		break;
	case 'a':
		angle += 0.1;
		break;
	case 'e':
		useShader = !useShader;
		if (useShader) {
			glUseProgram(program);
		} else {
			glUseProgram(NULL);
		}
		break;
	}
	displayCallback();
}

void specialCallback(int key, int, int)
{
	switch (key) {
	default:
		scene->transform = Transform::scale({ 6.0, 6.0, 6.0 });
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
	++frameCount;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - lastFrameTime > 1000) {
		std::cout << "FPS: " << frameElapsed * 1000 / (time - lastFrameTime) << '\n';
		lastFrameTime = time;
		frameElapsed = 0;
	}
	angle += 0.05;
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
}

int main(int argc, char** argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Simple Water");

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

void initScene()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glCullFace(GL_BACK);

	Vector3 center(0, 0, 5);
	Vector3 eye(0, 0, 0);
	Vector3 up(0, 1, 0);
	camera->transform = Transform::lookAt(center, eye, up);

	skybox->loadTextures(skyboxTextures);
	skybox->transform = Transform::scale({ 0.5, 0.5, 0.5 });

	scene->transform = Transform::scale({ 6.0, 6.0, 6.0 });

	//light->transform = Transform::translate({ 0.0, 0.9, -0.5 });

	camera->attach(scene);
	scene->attach(skybox);
	scene->attach(light);
	scene->attach(suit);
}