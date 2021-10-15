#include "PlayScene.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
#include <cmath>
#include <glm/gtx/string_cast.hpp>

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


	m_pPixelScaleLabel->setText("Pixels Per Meter = " + std::to_string(PPM));

	m_pMassScaleLabel->setText("Mass = " + std::to_string(m_Mass));

	m_pGravitycaleLabel->setText("Gravity = " + std::to_string(m_gravity));

	m_pVelocityScaleLabel->setText("Velocity = " + std::to_string(m_Velocity));

	m_pAccelerationScaleLabel->setText("Acceleration = " + std::to_string(m_Acceleration));

	m_pForceScaleLabel->setText("Force = " + std::to_string(m_Force));


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
	reset();
	physics();

	
	
	
	rampHeight = 500 - rampY;
	rampLength = rampX - 50;
	//Calculate ramp angle
	int rampAngle = (atan(rampHeight/rampLength) * 180/3.14);

	
	if(crate->getTransform()->position.y < groundLv - crate->getHeight() / 2)
	{
		crate->setCurrentHeading(rampAngle);
	}
	else
	{
		crate->setCurrentHeading(0);
	}
	
	//std::cout << rampAngle << std::endl;
	
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
		isMoving = true;
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Simulation Variables";
	
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
	
	m_pPixelScaleLabel = new Label("1 Pixel = 1 Meter", "Consolas", 17, { 255,0,0,255 }, glm::vec2(900.0f, 22.0f));
	addChild(m_pPixelScaleLabel);
	
	m_pMassScaleLabel = new Label("1 Mass = 1 Meter", "Consolas", 17, { 255,0,0,255 }, glm::vec2(900.0f, 42.0f));
	addChild(m_pMassScaleLabel);

	m_pGravitycaleLabel = new Label("1 Mass = 1 Meter", "Consolas", 17, { 255,0,0,255 }, glm::vec2(900.0f, 62.0f));
	addChild(m_pGravitycaleLabel);
	
	m_pVelocityScaleLabel = new Label("1 Mass = 1 Meter", "Consolas", 17, { 255,0,0,255 }, glm::vec2(900.0f, 82.0f));
	addChild(m_pVelocityScaleLabel);

	m_pAccelerationScaleLabel = new Label("1 Mass = 1 Meter", "Consolas", 17, { 255,0,0,255  }, glm::vec2(900.0f, 102.0f));
	addChild(m_pAccelerationScaleLabel);

	m_pForceScaleLabel = new Label("1 Mass = 1 Meter", "Consolas", 17, { 255,0,0,255  }, glm::vec2(900.0f, 122.0f));
	addChild(m_pForceScaleLabel);

	


	
	
	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	////ImGui::ShowDemoWindow();
	ImGui::Begin("Controls for Simulation", nullptr,  ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
	ImGui::Checkbox("Reset", &resetVariables);
	//ImGui::SliderFloat("Ramp XPosition", &rampX, 0, 200, "%.3f");
	//ImGui::SliderFloat("Ramp Height", &rampRise, 0, 200, "%.3f");
	//ImGui::SliderFloat("Ramp Length", &rampRun, 0.f, 90.0f, "%.3f");
	
	
	ImGui::SliderFloat("Time", &time, 0.f, 20.0f, "%.3f");
	ImGui::SliderFloat("TimeScale", &timeScale, 0.f, 2.0f, "%.3f");
	ImGui::SliderFloat("Pixels Per Meter", &PPM, 0.f, 10.0f, "%.3f");
	ImGui::SliderFloat("Crate Mass", &m_Mass, 0.f, 100.0f, "%.3f");
	ImGui::SliderFloat("Crate Velocity", &m_Velocity, 0.f, 100.0f, "%.3f");
	ImGui::SliderFloat("Crate Force", &m_Force, 0.f, 100.0f, "%.2f");
	ImGui::SliderFloat("Acceleration", &m_Acceleration, 0.f, 100.0f, "%.3f");
	ImGui::SliderFloat("Gravity", &m_gravity, 0.f, 50.0f, "%.3f");
	ImGui::SliderFloat("Kinetic Friction", &m_kineticFriction, 0.f, 10.0f, "%.3f");
	ImGui::SliderFloat("Ramp X", &rampX, 50.f, 600.0f, "%.3f");
	ImGui::SliderFloat("Ramp Y", &rampY, 1.0f, 500.0f, "%.3f");

	ImGui::Separator();

	
	ImGui::Text("Simulation Type");
	if(ImGui::Checkbox("Simulate", &simulateStart))
	{
		// toggle grid on/off
		simulateStart = true;
		run = true;
	}
	ImGui::StyleColorsClassic();
	ImGui::Separator();
	ImGui::End();

	
}

void PlayScene::simulate()
{
		
}

void PlayScene::physics()
{
	float dt = Game::Instance().getDeltaTime();

	if(isMoving == false)
	{
		crate->getTransform()->position = glm::vec2(60, rampY - crate->getHeight() /2 );
	}
	else if(isMoving == true && crate->getTransform()->position.y < groundLv - crate->getHeight() / 2)
	{
		//return radian of ramp angle to use in sin/cos calculations
		float Angle = atan(rampHeight/rampLength);
		time += dt * timeScale;

		//calculate acceleration against the angle of the normal and gravity
		m_Acceleration += (m_gravity) * sin(Angle);
	
		//applying acceleration to crate properties and multiplying using formula to find acceleration velocity
		crate->getRigidBody()->acceleration = glm::vec2(m_Acceleration * cos(Angle), m_Acceleration * sin(Angle));
		//std::cout << crate->getRigidBody()->acceleration.x << std::endl << crate->getRigidBody()->acceleration.y << std::endl;
		//std::cout << "Velocity X " << crate->getRigidBody()->velocity.x << std::endl << "Velocity Y " << crate->getRigidBody()->velocity.y << std::endl;
		
		//applying acceleration to velocity and applying delta and PPM parameter
		crate->getRigidBody()->velocity += crate->getRigidBody()->acceleration * dt * PPM;

		//applying velocity to crate position every frame multiplyed by deltatime.
		crate->getTransform()->position += crate->getRigidBody()->velocity * dt;
	}
	else
	{
		
	}
}

void PlayScene::reset()
{

	if(resetVariables)
	{
		time = 0;
		m_Mass = 12.8;
		m_Speed = 25;
	    PPM = 1;
	    m_kineticFriction = 0.41;
	    m_gravity = 9.8;
      	m_normalForce = 9.8;
		rampX = 300;
	    rampY = 250;
		timeScale = 1;
		m_Velocity = 0;
		m_Acceleration = 0;
		isMoving = false;
		resetVariables = false;

		crate->getRigidBody()->velocity = glm::vec2(0, 0);
	}
}

void PlayScene::drawTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 160, 82, 45, 1);
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

