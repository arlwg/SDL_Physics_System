#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Game.h"
#include "Scene.h"
#include "Wookie.h"
#include "Button.h"
#include "Label.h"
#include "ThermalDetonator.h"
#include "Background.h"
#include "StormTrooper.h"

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

	Background* bg;
	Wookie* wookie;
	ThermalDetonator *bomb;
	//default grenade velocities
	float vX = 91.3704;
	float vY = -26.008;
	//secondary values

	//speed
	float speed = 95.0f;
	//grenade angle
	float angle;
	//gravity
	int gravity = 9.8;
	//stormtrooper x and y
	float sY;
	float sX;

	float counter1 = 0;
	bool run = true;
	bool startCount = false;
	//is player facing right
	bool m_playerFacingRight;
	// For Situation A ( Stormtrooper 485M away )
	bool simulateStart = false;

	// UI Items


	Button* m_pBackButton;
	Button* m_pNextButton;

	Label* m_pInstructionLabel;

	Label* m_pPixelScaleLabel;

	Label* m_Hit;

private: 
	void simulate();

};

#endif /* defined (__PLAY_SCENE__) */