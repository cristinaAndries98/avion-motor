#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

	friend class Plane;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	void DrawPlane(float translateY, float rotation, float deltaTimeSeconds);
	void DrawClouds(float translateX, float translateY, float deltaTimeSeconds);
	void DrawObstacles(float obstacleX, float obstacleY, float deltaTimeSeconds);
	void DrawFuel(float fuelX, float fuelY, float deltaTimeSeconds);
	void DrawSea(float deltaTimeSeconds);
	void PlaneCrash(float deltaTimeSeconds);

	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat4 modelMatrix;
	float translateX, translateY, translateZ;
	float scaleX, scaleY, scaleZ;
	float angularStepOX, angularStepOY, angularStepOZ;
	float cloudX1, cloudY1, cloudX2, cloudY2, cloudX3, cloudY3;
	float obstacleY1, obstacleY2, obstacleY3, obstacleY4, obstacleY5;
	float obstacleX1, obstacleX2, obstacleX3, obstacleX4, obstacleX5;
	float fuelX1, fuelY1, fuelX2, fuelY2, fuelX3, fuelY3;
	float rotation;
	float viteza, speed, fallSpeed, flyingUp;
	float fuel;
	int lives;
	int tx;
	bool endgame;
	//variabile pentru camera
	Laborator::CameraTema* camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;
	GLenum polygonMode;
};
