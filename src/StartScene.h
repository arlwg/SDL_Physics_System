#pragma once
#ifndef __START_SCENE__
#define __START_SCENE__

#include "Game.h"
#include "Scene.h"
#include "Label.h"
#include "Button.h"
#include "Background.h"

class StartScene final : public Scene
{
public:
	StartScene();
	~StartScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	
private:
	Label* m_pStartLabel{};
	Label* m_pInstructionsLabel{};
	Label* m_IDLabel{};
	
	

	Button* m_pStartButton;
};

#endif /* defined (__START_SCENE__) */