#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Game.h"
#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Crate.h"
#include "Ramp.h"
#include "Background.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;
	

	float time = 0;
	float timeScale = 1;


	//Objs
	Ramp* ramp;
	Background* bg;
	Crate* crate;



	//Ramp Vars
	float rampX = 300;
	float rampY = 250;

	float rampHeight;
	float rampLength;
	//Ground Level
	float groundLv = 500;
	
	//Simulation variables

	float m_kineticFriction = 0.41;
	float m_gravity = 9.8;
	float m_normalForce = 9.8;
	//Pixels Per Meter
	int PPM = 1;

	

	//Crate Variables

	float m_Velocity;
	
	float m_Mass = 12.8;

	


	

	//Counters?
	float counter1 = 0;
	bool run = true;
	bool startCount = false;

	//Is the simulation running
	
	bool simulateStart = false;


	bool resetVariables;
	//Is the crate in motion
	
	bool isMoving = false;
	
	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;

	Label* m_pInstructionLabel;

	Label* m_pPixelScaleLabel;
	Label* m_pGravitycaleLabel;
	Label* m_pMassScaleLabel;

	Label* m_Hit;

private: 
	void simulate();
	void reset();
	void drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);
	
	
};

#endif /* defined (__PLAY_SCENE__) */