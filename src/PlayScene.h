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


	//Crate Vars
	float gravity = 100;

	//Ramp Vars
	float rampRise;
	float rampRun;
	float groundLv = 500;
	

	float counter1 = 0;
	bool run = true;
	bool startCount = false;
	

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