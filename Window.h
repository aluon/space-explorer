#pragma once
#include "Transform.h"
#include "Matrix4.h"

void displayCallback();
Vector3 mapMousePos(int x, int y);
void loadModel(const std::string &modelPath);
void initScene();
void centerAndFitModel();
std::string loadShader(const std::string &shaderPath);
void specialCallback(int key, int, int);
void exitFullScreen();