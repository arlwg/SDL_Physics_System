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

PlayScene::~PlayScene() = default;

void PlayScene::draw()
{
	drawDisplayList();
	crate->draw();
	
	//DrawLaunchPoint
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);
	
	//Draw Ground
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(),0,groundLv, 1000, groundLv);


	//Draw Ramp
	//vertical part of ramp
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 50, 500, 50, rampY);
	//bottom part of ramp
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 50, 500, rampX, 500);
	//slope of ramp
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 50, rampY, rampX, 500);
	
	
	//Fill Ramp
	drawTriangle(glm::vec2(50, rampY), glm::vec2(50, 500), glm::vec2(rampX, 500));
	

	
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	simulate();

	//Calculates Time
	float dt = Game::Instance().getDeltaTime();
	time += dt * timeScale;

	rampHeight = 500 - rampY;
	rampLength = rampX - 50;
	//Calculate ramp angle
	int rampAngle = (atan(rampHeight/rampLength) * 180/3.14);
	if(crate->getTransform()->position.x <= rampLength - crate->getHeight() / 2)
	{
		crate->setCurrentHeading(rampAngle);
	}
	else
	{
		crate->setCurrentHeading(0);
	}
	
	std::cout << rampAngle << std::endl;
	crate->getTransform()->position = glm::vec2(60, rampY - crate->getHeight() /2 );
	//Adds gravity to crate and stops on the ground
	//if (CollisionManager::lineRectCheck(glm::vec2(0, groundLv), glm::vec2(1000, groundLv),crate->getTransform()->position, (float)crate->getWidth(),(float)crate->getHeight()))
	//{
	//	
	//}
	//else
	//{
	//	crate->getTransform()->position.y += gravity * pow(time, 2);
	//}

	
	
	crate->update();
	ramp->update();

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
	
	rampHeight = 500 - rampY;
	rampLength = rampX - 50;
	
	crate = new Crate();
	addChild(crate);
	crate->getTransform()->position = glm::vec2(50 + crate->getWidth()/2, rampHeight - crate->getHeight() / 2);
	



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
	//ImGui::SliderFloat("Ramp XPosition", &rampX, 0, 200, "%.3f");
	//ImGui::SliderFloat("Ramp Height", &rampRise, 0, 200, "%.3f");
	//ImGui::SliderFloat("Ramp Length", &rampRun, 0.f, 90.0f, "%.3f");
	
	
	ImGui::SliderFloat("Time", &time, 0.f, 20.0f, "%.3f");
	ImGui::SliderFloat("TimeScale", &timeScale, 0.f, 2.0f, "%.3f");
	ImGui::SliderFloat("Ramp X", &rampX, 50.f, 600.0f, "%.3f");
	ImGui::SliderFloat("Ramp Y", &rampY, 0.f, 500.0f, "%.3f");

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

void PlayScene::drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 160,82,45, 1);
  float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
  float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

  float curx1 = v1.x;
  float curx2 = v1.x;

  for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
  {
    SDL_RenderDrawLine(Renderer::Instance().getRenderer(), (int)curx1, scanlineY, (int)curx2, scanlineY);
    curx1 += invslope1;
    curx2 += invslope2;
  }
}
