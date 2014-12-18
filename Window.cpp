#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <Soil/SOIL.h>
#include <iostream>
#include <algorithm>
#include "Window.h"
#include "Geode.h"
#include "Light.h"
#include "Skybox.h"
#include "Shader.h"
#include "Parser.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "ShipTransform.h"

namespace
{
	int windowWidth = 512, windowHeight = 512;
	const double zNear = 0.1, zFar = 1000.0, fov = 60.0;

	double frame = 0.0, frameElapsed = 0, lastFrameTime = 0.0;

	bool fullscreen = false;

	auto scene = std::make_shared<MatrixTransform>();
	auto camera = std::make_shared<Camera>();
	auto sphere = std::make_shared<Sphere>();
	auto cone = std::make_shared<Cone>();
	auto light = std::make_shared<Light>(GL_LIGHT0);
	auto skybox = std::make_shared<Skybox>();
	auto tailEmitter = std::make_shared<ParticleEmitter>(1000);
	auto leftGun = std::make_shared<ParticleEmitter>(10);
	auto rightGun = std::make_shared<ParticleEmitter>(10);
	auto ship = parseModel("models/arc170lp.obj");
	auto shipTransform = std::make_shared<ShipTransform>();

	Vector3 center(0, 0, 1.5);
	Vector3 eye(0, 0, 0);
	Vector3 up(0, 1, 0);

	Vector3 initialMousePos;
	
	bool useShader = true;
	bool rocketsOn = false;
	bool lmbDown = false;
	bool rmbDown = false;

	GLuint program;
	GLuint shipShaderProgram = 0, skyboxShaderProgram = 0;

	double rollAngle = 0.0;
}

double aspect()
{
	if (windowHeight == 0) {
		return 1.0;
	}
	return static_cast<double>(windowWidth) / windowHeight;
}

Vector3 mapMousePos(int x, int y)
{
	Vector3 v((2.0 * x - windowWidth) / windowWidth, (windowHeight - 2.0 * y) / windowHeight, 0.0);
	double d = v.length();
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrt(1.001 - d * d);
	return v.normalize();
}

void mouseCallback(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		initialMousePos = mapMousePos(x, y);
		lmbDown = true;
	} else {
		lmbDown = false;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		initialMousePos = mapMousePos(x, y);
		rmbDown = true;
	} else {
		rmbDown = false;
	}
}


void motionCallback(int x, int y)
{
	Vector3 mousePos = mapMousePos(x, y);
	if (lmbDown) {
		Vector3 direction = mousePos - initialMousePos;
		double velocity = direction.length();
		if (velocity > 0.0001) {
			Vector3 axis = cross(initialMousePos, mousePos);
			double angle = velocity;
			camera->rotMatrix *= Transform::rotate(angle, axis);
		}
	}

	if (rmbDown) {
		double dx = mousePos.x - initialMousePos.x + mousePos.y - initialMousePos.y;
		double zoom = dx / 5;
		camera->transform *= Transform::translate(Vector3(0.0, 0.0, zoom));
	}
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
	const double dr = 2.0, ds = 0.1, dt = 0.5, ry = 1.0;
	Vector3 forward = (eye - center).normalize() * 0.1;
	switch (key) {
	case 'z':
		camera->rotMatrix *= Transform::rotateX(dr);
		break;
	case 'Z':
		camera->rotMatrix *= Transform::rotateX(-dr);
		break;
	case 'x':
		camera->rotMatrix *= Transform::rotateY(dr);
		break;
	case 'X':
		camera->rotMatrix *= Transform::rotateY(-dr);
		break;
	case 'c':
		camera->rotMatrix *= Transform::rotateZ(dr);
		break;
	case 'C':
		camera->rotMatrix *= Transform::rotateZ(-dr);
		break;
	case 's':
		camera->center += forward;
		camera->eye += forward;
		break;
	case 'S':
		camera->center -= forward;
		camera->eye -= forward;
		break;
	case 'w':
		shipTransform->pitch += dt;
		camera->rotMatrix *= Transform::rotateX(-ds);
		break;
	case 'a':
		shipTransform->roll += dt;
		camera->rotMatrix *= Transform::rotateY(-ry);
		break;
	case 'd':
		shipTransform->roll -= dt;
		camera->rotMatrix *= Transform::rotateY(ry);
		break;
	case 'e':
		useShader = !useShader;
		if (useShader) {
			ship->programId = shipShaderProgram;
			skybox->programId = skyboxShaderProgram;
			std::cout << "Shaders on" << '\n';
		} else {
			ship->programId = 0;
			skybox->programId = 0;
			std::cout << "Shaders off" << '\n';
		}
		break;
	case 32:	// space
		leftGun->enabled = !leftGun->enabled;
		rightGun->enabled = !rightGun->enabled;
		break;
	case 8:		// backspace
		camera->transform = Transform::lookAt(center, eye, up);
		shipTransform->reset();
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
	camera->reset(center, eye, up);
	camera->rotMatrix = Transform::rotateX(15.0);
	Vector3 forward = (center - eye).normalize();
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
	shipShaderProgram = glCreateProgram();
	glAttachShader(shipShaderProgram, loadShader(GL_VERTEX_SHADER, "shaders/normalmapping.vert"));
	glAttachShader(shipShaderProgram, loadShader(GL_FRAGMENT_SHADER, "shaders/normalmapping.frag"));
	glLinkProgram(shipShaderProgram);
	ship->programId = shipShaderProgram;

	skyboxShaderProgram = glCreateProgram();
	glAttachShader(skyboxShaderProgram, loadShader(GL_VERTEX_SHADER, "shaders/skybox.vert"));
	glAttachShader(skyboxShaderProgram, loadShader(GL_FRAGMENT_SHADER, "shaders/skybox.frag"));
	glLinkProgram(skyboxShaderProgram);
	skybox->programId = skyboxShaderProgram;
}

int main(int argc, char** argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Planets");

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
	auto shipTexture = SOIL_load_OGL_texture
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

	auto normalTexture = SOIL_load_OGL_texture
		(
		"assets/arc170n2.tga",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	if (!normalTexture) {
		std::cout << "SOIL loading error: " << SOIL_last_result() << '\n';
	}
	ship->normalMapId = normalTexture;

	auto fireTexture = SOIL_load_OGL_texture
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

	auto projectileTexture = SOIL_load_OGL_texture
		(
		"assets/weapon_flash2.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	if (!projectileTexture) {
		std::cout << "SOIL loading error: " << SOIL_last_result() << '\n';
	}
	leftGun->textureId = projectileTexture;
}

void initScene()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	GLfloat lightPos[] = { 0.0, 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	camera->center = center;
	camera->eye = eye;
	camera->up = up;
	camera->rotMatrix = Transform::rotateX(15.0);

	skybox->transform = Transform::scale(100);

	loadTextures();
	tailEmitter->particleRadius = 3.0;
	tailEmitter->transform = Transform::scale(0.05) * Transform::rotateX(90.0) * Transform::translate({ 0.0, 6.0, 0.0 });
	tailEmitter->emitRate = 20;
	tailEmitter->enabled = true;

	leftGun->transform = Transform::scale({0.05, 0.05, 0.85}) * Transform::rotateX(-90.0);
	leftGun->particleRadius = 0.1;

	camera->attach(scene);
	scene->attach(skybox);
	scene->attach(light);
	scene->attach(shipTransform);
	shipTransform->attach(ship);
	shipTransform->attach(tailEmitter);
	shipTransform->attach(leftGun);
	//shipTransform->attach(rightGun);
}