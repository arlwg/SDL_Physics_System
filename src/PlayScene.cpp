#include "PlayScene.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
#include <cmath>
#include "CollisionManager.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
	bomb->draw();
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(),0,500, 1000, 500);
}

void PlayScene::update()
{
	/*INFORMATION :
	Launch velocity 95 m/s
	Angle 45 degrees

	Stormtrooper Position :
	X = 485
	Y = 0

	MAX RANGE PROBLEM :

	ANGLE : 45
	Initial Velocity = 95 m/s
	Initial Velocity(x) = 67.175 m/s
	Initial Velocity(y) = 67.175 m/s

	STORMTROOPER RANGE PROBLEM :

	ANGLE : 15.889
	Initial Velocity = 95 m/s
	Initial Velocity(x) = 91.3704 m/s
	Initial Velocity(y) = 26.008 m/s */



	updateDisplayList();

	simulate();

	float dt = Game::Instance().getDeltaTime();
	//time
	if(run)
	time += dt * timeScale;
	if (bomb->getTransform()->position.y >= 600)
	{
		run = false;
	}
	//updating velocity with acceleration ( gravity )
	bomb->getRigidBody()->velocity += bomb->getRigidBody()->acceleration * time;
	//updating position according to updated velocity
	bomb->getTransform()->position += bomb->getRigidBody()->velocity * time;

	bomb->update();

	
	// angle of velocity (vx divided by vy)  * 180 / Pi
	float theta = atan(vY / vX) * 180.0 / M_PI;

	
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement if no Game Controllers found

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		simulate;
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	bg = new Background();
	bg->type = 1;
	addChild(bg);
	bg->getTransform()->position = glm::vec2(500,300);

	wookie = new Wookie();
	addChild(wookie);
	wookie->getTransform()->position = glm::vec2(25.0f, 400.0f);

	bomb = new ThermalDetonator();
	bomb->getTransform()->position = glm::vec2(25.0f, 400.0f);
	addChild(bomb);
	



	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(40.0f, 25.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance().changeSceneState(START_SCENE);
	});

	addChild(m_pBackButton);


	m_Hit = new Label("\0", "Consolas", 20, { 255,255,0,255 }, glm::vec2(0.0f, 0.0f));
	addChild(m_Hit);
	/* Instructions Label */
	/* = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");*/
	m_pInstructionLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 15, { 255,255,255,255 }, glm::vec2(Config::SCREEN_WIDTH * 0.5f, 22.0f));
	addChild(m_pInstructionLabel);

	m_pPixelScaleLabel = new Label("1 Pixel = 1 Meter", "Consolas", 17, { 255,255,255,255 }, glm::vec2(900.0f, 22.0f));

	addChild(m_pPixelScaleLabel);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);
	ImGui::SliderFloat("Speed", &speed, 0, 200, "%.3f");
	ImGui::SliderFloat("Time", &time, 0.f, 20.0f, "%.3f");
	ImGui::SliderFloat("TimeScale", &timeScale, 0.f, 2.0f, "%.3f");
	ImGui::SliderFloat("Angle", &angle, 0.f, 90.0f, "%.3f");
	ImGui::Separator();


	
	ImGui::Text("Simulation Type");
	if(ImGui::Checkbox("Simulate", &simulateStart))
	{
		// toggle grid on/off
		simulateStart = true;
		run = true;
	}

	ImGui::Separator();
	ImGui::End();
}

void PlayScene::simulate()
{
	if(simulateStart)
	{
		//Setting initial position of Bomb, Wookie, and Stormtrooper
		bomb->getTransform()->position = glm::vec2(60.0f, 500.0f);
		wookie->getTransform()->position = glm::vec2(60.0f, 500.0f);
		
		//485 + 25(initial position of wookie/bomb) = 510
		sX = 545.0f;
		sY = 500.0f;
		// Assigning Speed
		speed = 95;
		// Assigning Angle
		angle = 15.88963282;
		// Speed of Velocity X
		bomb->getRigidBody()->velocity.x = speed * cos(angle * M_PI / 180.0);
		// Speed of Velocity Y
		bomb->getRigidBody()->velocity.y = -speed * sin(angle * M_PI / 180.0);
		// Changing boolean to false so it doesn't reset values in update() during simulation
		time = 0;
		simulateStart = false;
	}
		

	
}
