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
	crate->draw();

	

	//Draw Ground
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(),0,groundLv, 1000, groundLv);
}

void PlayScene::update()
{
	



	updateDisplayList();

	simulate();

	float dt = Game::Instance().getDeltaTime();
	//time
	if(run)
	time += dt * timeScale;
	
	

	crate->update();

	
	

	
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
		simulate();
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

	crate = new Crate();
	addChild(crate);
	



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
	ImGui::SliderFloat("Ramp XPosition", &rampX, 0, 200, "%.3f");
	ImGui::SliderFloat("Ramp Height", &rampRise, 0, 200, "%.3f");
	ImGui::SliderFloat("Ramp Length", &rampRun, 0.f, 90.0f, "%.3f");
	
	
	ImGui::SliderFloat("Time", &time, 0.f, 20.0f, "%.3f");
	ImGui::SliderFloat("TimeScale", &timeScale, 0.f, 2.0f, "%.3f");

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
		//Simulation Code Starts here

		

		// Simulation  Code Ends here
		time = 0;
		simulateStart = false;
	}
		

	
}
