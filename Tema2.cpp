#include "Tema2.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	polygonMode = GL_FILL;
	glm::ivec2 resolution = window->GetResolution();

	renderCameraTarget = false;
	//pentu camera noua 
	camera = new Laborator::CameraTema();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	Mesh* mesh1 = new Mesh("sphere");
	mesh1->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh1->GetMeshID()] = mesh1;

	Mesh* mesh2 = new Mesh("teapot");
	mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
	meshes[mesh2->GetMeshID()] = mesh2;

	Mesh* mesh3 = new Mesh("sphere");
	mesh3->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh3->GetMeshID()] = mesh3;

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	//rotatie nori
	viteza = 0;

	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;

	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;

	//parametrii pentru nori
	speed = 0;
	cloudX1 = 2.5f;
	cloudY1 = 2.0f;
	cloudX2 = 2.5f;
	cloudY2 = 1.5f;
	cloudX3 = 2.5f;
	cloudY3 = 1.0f;

	//parametrii pentur obstacole
	obstacleY1 = 1.3f;
	obstacleY2 = 1.7f;
	obstacleY3 = 2.1f;
	obstacleY4 = 1.5f;
	obstacleY5 = 2.0f;

	obstacleX1 = 0;
	obstacleX2 = 0;
	obstacleX3 = 0;
	obstacleX4 = 1.5f;
	obstacleX5 = 2.0f;

	//parametrii pentru fuel
	fuelX1 = 2.5f;
	fuelY1 = 0.5f;
	fuelX2 = 2.5f;
	fuelY2 = 1.1f;
	fuelX3 = 2.5f;
	fuelY3 = 1.7f;
	//indice de deplasare
	tx = resolution.x;

	//numar de vieti
	lives = 3;

	//cantitate initiala de fuel
	fuel = 2.5f;

	//game status
	endgame = false;

	//viteza de cadere a avionului la final
	fallSpeed = 0;
	flyingUp = 0;
}

//functie utilizata pentru desenarea avionului din cuburi
//avionul are o elice care se roteste in continuu
//avionul are inclinatie dupa un factor de rotatie numit rotatioin
void Tema2::DrawPlane(float translateY, float rotation, float deltaTimeSeconds)
{
	//coada avionului
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, translateY, 0);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(-0.4f, 0, -1.5f);
	modelMatrix *= Transform3D::Scale(-0.2, -0.3, -0.3);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//corpul avionului
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, translateY, 0);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(-0.1f, 0, -1.5f);
	modelMatrix *= Transform3D::Scale(0.6, 0.6, 0.6);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//suport elice
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, translateY, 0);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(0.3f, 0, -1.5f);
	modelMatrix *= Transform3D::Scale(-0.2, -0.1, -0.1);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//elice
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, translateY, 0);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	angularStepOX += 10 * deltaTimeSeconds;
	modelMatrix *= Transform3D::Translate(0.4f, 0, -1.5f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);//elicea are o miscare de rotatie mereu
	modelMatrix *= Transform3D::Scale(-0.1, -0.6, -0.6);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//aripa dreapta 
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, translateY, 0);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(-0.05f, 0.2, -0.4f);
	modelMatrix *= Transform3D::Scale(-0.3, -0.2, -0.2);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//aripa stanga 
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, translateY, 0);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(-0.05f, 0.2f, -1.7f);
	modelMatrix *= Transform3D::Scale(-0.3, -0.2, -0.2);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

//functie de desenare a unui nor
//norii sunt generati de la dreapta la stanga in intervalul (-2.5; 2.5) pe axa OX 
void Tema2::DrawClouds(float cloudX, float cloudY, float deltaTimeSeconds)
{
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, -3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOZ(-cloudX);
	modelMatrix *= Transform3D::Translate(0.0f, cloudY, 0.0f);
	modelMatrix *= Transform3D::Translate(0.0f, 3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOZ(0.5);
	modelMatrix *= Transform3D::Scale(-0.09, -0.09, -0.09);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, -3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOZ(-cloudX);
	modelMatrix *= Transform3D::Translate(0.0f, cloudY, 0.0f);
	modelMatrix *= Transform3D::Translate(0.1f, 3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOX(1.5f * deltaTimeSeconds);
	modelMatrix *= Transform3D::Scale(0.07, 0.07, 0.07);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, -3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOZ(-cloudX);
	modelMatrix *= Transform3D::Translate(0.1f, cloudY + 0.09f, 0.0f);
	modelMatrix *= Transform3D::Translate(0.0f, 3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOX(1.5f * deltaTimeSeconds);
	modelMatrix *= Transform3D::RotateOZ(-0.5);
	modelMatrix *= Transform3D::Scale(0.05, 0.05, 0.05);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, -3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOZ(-cloudX);
	modelMatrix *= Transform3D::Translate(0.15f, cloudY + 0.05f, 0.0f);
	modelMatrix *= Transform3D::Translate(0.0f, 3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOZ(0.3);
	modelMatrix *= Transform3D::Scale(0.07, 0.07, 0.07);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

//desenam obstacolele pe ecran
void Tema2::DrawObstacles(float obstacleX, float obstacleY, float deltaTimeSeconds)
{
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, -3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOZ(-obstacleX);
	modelMatrix *= Transform3D::Translate(0.0f, obstacleY, 0.0f);
	modelMatrix *= Transform3D::Translate(0.0f, 3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::Scale(0.15, 0.15, 0.15);
	RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
}

//desenam fuel pe ecran
void Tema2::DrawFuel(float fuelX, float fuelY, float deltaTimeSeconds)
{
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, -3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOZ(-fuelX);
	modelMatrix *= Transform3D::Translate(0.0f, fuelY, 0.0f);
	modelMatrix *= Transform3D::Translate(0.0f, 3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::Scale(0.15, 0.15, 0.15);
	RenderMesh(meshes["teapot"], shaders["Color"], modelMatrix);

	//pastrez matricea anterioara si fac doar translatare
	modelMatrix *= Transform3D::Translate(0.8f, 0.0f, 0.0f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::RotateOX(90);
	RenderMesh(meshes["teapot"], shaders["Color"], modelMatrix);

	modelMatrix *= Transform3D::Translate(1.0f, 0.0f, 0.0f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX * 0.004f);
	RenderMesh(meshes["teapot"], shaders["Color"], modelMatrix);
}

//desenam marea pe ecran
void Tema2::DrawSea(float deltaTimeSeconds)
{
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, -3.5f, 0.0f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX * 0.04f);
	modelMatrix *= Transform3D::Scale(7.0f, 7.0f, 7.0f);
	RenderMesh(meshes["sphere"], shaders["Color"], modelMatrix);
}

//animatia de final
void Tema2::PlaneCrash(float deltaTimeSeconds)
{
	fallSpeed -= 0.004 * deltaTimeSeconds;
	flyingUp += 2 * fallSpeed;
	//printf("flyingup: %f\n", flyingUp);

	//coada avionului
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(-0.4f, translateY + flyingUp, -1.5f);
	modelMatrix *= Transform3D::RotateOZ(flyingUp);
	modelMatrix *= Transform3D::Scale(0.2, 0.3, 0.3);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//corpul avionului
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(0, translateY + flyingUp, 0);
	modelMatrix *= Transform3D::RotateOZ(flyingUp);
	modelMatrix *= Transform3D::Translate(-0.1f, 0, -1.5f);
	modelMatrix *= Transform3D::Scale(0.6, 0.6, 0.6);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//suport elice
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(0, translateY + flyingUp, 0);
	modelMatrix *= Transform3D::RotateOZ(flyingUp);
	modelMatrix *= Transform3D::Translate(0.3f, 0, -1.5f);
	modelMatrix *= Transform3D::Scale(-0.2, -0.1, -0.1);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//elice
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(0, translateY + flyingUp, 0);
	modelMatrix *= Transform3D::RotateOZ(flyingUp);
	angularStepOX += 10 * deltaTimeSeconds;
	modelMatrix *= Transform3D::Translate(0.4f, 0, -1.5f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);//elicea are o miscare de rotatie mereu
	modelMatrix *= Transform3D::Scale(-0.1, -0.6, -0.6);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//aripa dreapta
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(0, translateY + flyingUp, 0);
	modelMatrix *= Transform3D::RotateOZ(flyingUp);
	modelMatrix *= Transform3D::Translate(-0.05f, 0.2, -0.4f);
	modelMatrix *= Transform3D::Scale(-0.3, -0.2, -0.2);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	//aripa stanga
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::RotateOZ(rotation);
	modelMatrix *= Transform3D::Translate(0, translateY + flyingUp, 0);
	modelMatrix *= Transform3D::RotateOZ(flyingUp);
	modelMatrix *= Transform3D::Translate(-0.05f, 0.2f, -1.7f);
	modelMatrix *= Transform3D::Scale(-0.3, -0.2, -0.2);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	glm::ivec2 resolution = window->GetResolution();
	angularStepOZ -= deltaTimeSeconds / 4;

	//desenam marea
	DrawSea(deltaTimeSeconds);

	//daca se termina benzina pierdem o viata
	if (fuel < 0) {
		lives--;
		fuel = 2.0f;
	}
	if (lives == 0) {
		printf("Game over!\n");
		endgame = true;
		lives = -1;
		FrameEnd();
	}
	else if (lives == -1) {
		PlaneCrash(deltaTimeSeconds);
	}
	//daca am ajuns cu vietile la 0 se termina jocul
	if (endgame != true) {
		//desenam avionul 	
		DrawPlane(translateY, rotation, deltaTimeSeconds);

		//benzina se termina cu trecerea timpului
		fuel -= deltaTimeSeconds;
		printf("Fuel left: %f\n", fuel);

		//inainte de a apela DrawClouds facem deplasarea lor pe axa Ox 
		speed = 0.3f;
		cloudX1 -= speed * deltaTimeSeconds;
		if (cloudX1 + 2.5f < 0) cloudX1 = 2.5f;
		DrawClouds(cloudX1, cloudY1, deltaTimeSeconds);

		speed = 0.5f;
		cloudX2 -= speed * deltaTimeSeconds;
		if (cloudX2 + 2.5f < 0) cloudX2 = 2.5f;
		DrawClouds(cloudX2, cloudY2, deltaTimeSeconds);

		speed = 0.2f;
		cloudX3 -= speed * deltaTimeSeconds;
		if (cloudX3 + 2.5f < 0) cloudX3 = 2.5f;
		DrawClouds(cloudX3, cloudY3, deltaTimeSeconds);

		//desenam obstacole pe ecran
		speed = 1.0f;
		obstacleX1 -= speed * deltaTimeSeconds;
		if (obstacleX1 + 3.6f < 0) obstacleX1 = 3.6f;
		DrawObstacles(obstacleX1, obstacleY1, deltaTimeSeconds);

		speed = 0.4f;
		obstacleX2 -= speed * deltaTimeSeconds;
		if (obstacleX2 + 2.5f < 0) obstacleX2 = 2.5f;
		DrawObstacles(obstacleX2, obstacleY2, deltaTimeSeconds);

		speed = 0.3f;
		obstacleX3 -= speed * deltaTimeSeconds;
		if (obstacleX3 + 2.5f < 0) obstacleX3 = 2.5f;
		DrawObstacles(obstacleX3, obstacleY3, deltaTimeSeconds);

		speed = 0.5f;
		obstacleX4 -= speed * deltaTimeSeconds;
		if (obstacleX4 + 2.5f < 0) obstacleX4 = 2.5f;
		DrawObstacles(obstacleX4, obstacleY4, deltaTimeSeconds);

		speed = 0.4f;
		obstacleX5 -= speed * deltaTimeSeconds;
		if (obstacleX5 + 2.5f < 0) obstacleX5 = 2.5f;
		DrawObstacles(obstacleX5, obstacleY5, deltaTimeSeconds);

		//desenam combustibil pe ecran
		speed = 0.5f;
		fuelX1 -= speed * deltaTimeSeconds;
		if (fuelX1 + 2.5f < 0) fuelX1 = 2.5f;
		DrawFuel(fuelX1, fuelY1, deltaTimeSeconds);

		speed = 0.7f;
		fuelX2 -= speed * deltaTimeSeconds;
		if (fuelX2 + 2.5f < 0) fuelX2 = 2.5f;
		DrawFuel(fuelX2, fuelY2, deltaTimeSeconds);

		speed = 0.3f;
		fuelX3 -= speed * deltaTimeSeconds;
		if (fuelX3 + 2.5f < 0) fuelX3 = 2.5f;
		DrawFuel(fuelX3, fuelY3, deltaTimeSeconds);
	}
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(deltaTime * cameraSpeed);
		}

		//pentru proiectia ortografica
		if (window->KeyHold(GLFW_KEY_O)) {
			//left right down up
			projectionMatrix = glm::ortho(-8.0f, 8.0f, 4.5f, -4.5f, 0.0f, 200.0f);
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_P) {
		projectionMatrix = glm::perspective(90.f, 2.f, 2.f, 200.0f);
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

//deplasarea sus-jos a avionului depinde de mouse
void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (endgame != true) {
		glm::ivec2 resolution = window->GetResolution();
		float speed = 0.004f;
		translateY += -speed * deltaY;
		angularStepOZ = 0.5;

		//calculam inclinatia pasarii in functie de pozitia mouse-ului
		//rotation este factorul cu care se inclina pasarea
		if (deltaY > 0)
			rotation = -0.3f;
		else rotation = 0.3f;
	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
