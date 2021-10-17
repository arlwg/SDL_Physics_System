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

	

	//Draw Lines

	
	//DrawLaunchPoint
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);
	
	//Draw Ground
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(),0,groundLv, Config::SCREEN_WIDTH, groundLv);


	//Draw Ramp
	//vertical part of ramp
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 50, 500, 50, 500 - rampY * PPM);
	//bottom part of ramp
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 50, 500, rampX * PPM, 500);
	//slope of ramp
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 50, 500 - rampY * PPM, rampX * PPM + 50, 500);
	
	
	m_Force = m_kineticFriction * m_Mass * m_gravity;
	m_GravityForce = m_Mass * m_gravity;

	//Fill Ramp
	drawTriangle(glm::vec2(50, 500 - rampY * PPM), glm::vec2(50, 500), glm::vec2(rampX * PPM + 50, 500));
	
	if(crate->getTransform()->position.y < groundLv - crate->getHeight() / 2)
	{
	
		m_pForceScaleLabel->setText(" ");
		m_pGForceScaleLabel->setText(" ");

		
	}
	else
	{
		//Gravity Force
		m_pGForceScaleLabel->setText(" FG = "  + std::to_string(m_GravityForce));
		m_pGForceScaleLabel->getTransform()->position = crate->getTransform()->position -  glm::vec2(0, -m_GravityForce/2 );
		//Kinetic Drag Force
		m_pForceScaleLabel->getTransform()->position = crate->getTransform()->position -  glm::vec2(m_Force, 0 );
		m_pForceScaleLabel->setText("FK = " + std::to_string(m_Force));
		//Velocity Force
		Util::DrawLine(crate->getTransform()->position, crate->getTransform()->position - glm::vec2((m_Force), 0), glm::vec4(1, 0, 1 ,0));
	}
	//Draw lines for gravity and PPM
	Util::DrawLine(crate->getTransform()->position, crate->getTransform()->position - glm::vec2(0, -m_GravityForce/2), glm::vec4(0, 1, 0, 1));
	Util::DrawLine(glm::vec2(1050, 550), glm::vec2(1050 + PPM, 550), glm::vec4(1, 0, 1 ,0) );
		m_pPixelScaleLabel->setText("1 Meter =  " + std::to_string(PPM) + " Pixels");
	
}

void PlayScene::update()
{
	updateDisplayList();

	simulate();
	reset();
	physics();
	crate->update();
	//Calculates Time
	float dt = Game::Instance().getDeltaTime();
	std::cout << "Height :" << rampHeight << std::endl;
	std::cout <<"Length :" << rampLength << std::endl;
	rampHeight = rampY * PPM;
	rampLength = rampX * PPM;

	//Calculate velocity for Label
	m_Velocity = Util::magnitude(crate->getRigidBody()->velocity);

	//Calculate ramp angle
	int rampAngle = (atan(rampHeight/rampLength) * 180/3.14);
	Angle = atan(rampHeight/rampLength);
	//Updating Labels
	
	
	if(crate->getTransform()->position.y < groundLv - crate->getHeight() / 2)
	{
		crate->setCurrentHeading(rampAngle);
	
	}
	else
	{
		crate->setCurrentHeading(0);
		
	}
	

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);

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
		simulateStart = true;
		run = true;
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Simulation Variables";
	
	bg = new Background();
	bg->type = 1;
	addChild(bg);
	bg->getTransform()->position = glm::vec2(700, 300);
	rampHeight = rampY * PPM;
	rampLength = rampX * PPM ;
	std::cout << "RampLength " <<  rampLength << std::endl;
	std::cout << "RampHeight " <<  rampHeight << std::endl;
	crate = new Crate();
	addChild(crate);
	crate->getTransform()->position = { 110 + crate->getWidth() / 2, (500 - rampY * PPM) - crate->getHeight() / 2 };
	



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
	m_pInstructionLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 18, { 50,255,120,255 }, glm::vec2(Config::SCREEN_WIDTH * 0.5f, 22.0f));
	addChild(m_pInstructionLabel);
	m_pInstructionLabel = new Label("Press Spacebar to start simulation.", "Consolas", 22, { 0,255,255,255 }, glm::vec2(Config::SCREEN_WIDTH * 0.5f, 44.0f));
	addChild(m_pInstructionLabel);
	
	m_pPixelScaleLabel = new Label("1 Pixel = 1 Meter", "Consolas", 17, { 255,0,0,255 }, glm::vec2(1100.0f,575.0f));
	addChild(m_pPixelScaleLabel);
	
	m_pMassScaleLabel = new Label("1 Mass = 1 Meter", "Consolas", 17, { 0,255,255,255}, glm::vec2(1100.0f, 42.0f));
	addChild(m_pMassScaleLabel);

	m_pGravitycaleLabel = new Label("1 Mass = 1 Meter", "Consolas", 17, { 0,255,255,255 }, glm::vec2(1100.0f, 62.0f));
	addChild(m_pGravitycaleLabel);
	
	m_pVelocityScaleLabel = new Label("1 Mass = 1 Meter", "Consolas", 17, { 0,255,255,255 }, glm::vec2(1100.0f, 82.0f));
	addChild(m_pVelocityScaleLabel);

	m_pAccelerationScaleLabel = new Label("1 Mass = 1 Meter", "Consolas", 17, { 255,255,255,255  }, glm::vec2(1100.0f, 102.0f));
	addChild(m_pAccelerationScaleLabel);

	m_pForceScaleLabel = new Label(" ", "Consolas", 17, { 0,255,255,255  }, glm::vec2(crate->getTransform()->position.x - getWidth() + 50, crate->getTransform()->position.y - getHeight() / 2));
	addChild(m_pForceScaleLabel);


	m_pGForceScaleLabel = new Label(" ", "Consolas", 17, { 0,255,255,255  }, glm::vec2(crate->getTransform()->position.x - getWidth() + 50, crate->getTransform()->position.y - getHeight() / 2));
	addChild(m_pGForceScaleLabel);

	m_pAForceScaleLabel = new Label(" ", "Consolas", 17, { 0,255,255,255  }, glm::vec2(crate->getTransform()->position.x - getWidth() + 50, crate->getTransform()->position.y - getHeight() / 2));
	addChild(m_pAForceScaleLabel);


	
	
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
	ImGui::SliderFloat("TimeScale", &m_timeScale, 0.f, 20.f, "%.3f");
	ImGui::SliderFloat("Pixels Per Meter", &PPM, 1.f, 140.0f, "%.3f");
	ImGui::SliderFloat("Crate Mass", &m_Mass, 0.f, 100.0f, "%.3f");
	ImGui::SliderFloat("Crate Velocity", &m_Velocity, 0.f, 100.0f, "%.3f");
	ImGui::SliderFloat("Acceleration", &m_Acceleration, 0.f, 100.0f, "%.3f");
	ImGui::SliderFloat("Gravity", &m_gravity, 0.f, 50.0f, "%.3f");
	ImGui::SliderFloat("Kinetic Friction", &m_kineticFriction, 0.f, 10.0f, "%.3f");
	ImGui::SliderFloat("Ramp X", &rampX, 1.0f, 8.0f, "%.3f");
	ImGui::SliderFloat("Ramp Y", &rampY, 0.2f, 4.5f, "%.3f");

	ImGui::Separator();

	
	ImGui::Text("Simulation Type");
	if(ImGui::Checkbox("Simulate", &simulateStart))
	{
		// toggle grid on/off
		isMoving = true;
		simulateStart = true;
		run = true;
	}
	ImGui::StyleColorsClassic();
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
		isMoving = true;
		simulateStart = false;
	}
		

	
}

void PlayScene::physics()
{
	
	float dt = Game::Instance().getDeltaTime();


	
	if(!isMoving)
	{
		crate->getTransform()->position = glm::vec2(60, (500 - rampY * PPM) - crate->getHeight() /2 );
	}
	else if(isMoving && crate->getTransform()->position.y < groundLv - crate->getHeight() / 2)
	{
		//return radian of ramp angle to use in sin/cos calculations
		
		float angletest = Angle * 180 / 3.14;

		
		time += dt * m_timeScale;

		//calculate acceleration against the angle of the ramp and gravity
		m_Acceleration = m_gravity * sin(Angle);
	
		//applying acceleration to crate properties and multiplying using velocity formula to find acceleration velocity for the different planes
		crate->getRigidBody()->acceleration = glm::vec2(m_Acceleration * cos(Angle), m_Acceleration * sin(Angle));
		//applying acceleration to velocity and applying time and PPM parameter
		crate->getRigidBody()->velocity += crate->getRigidBody()->acceleration * dt * (PPM/PPM);

		//applying velocity to crate position every frame multiplyed by deltatime.
		crate->getTransform()->position += crate->getRigidBody()->velocity * m_timeScale;
	}
	else if (isMoving && crate->getTransform()->position.y >= groundLv - crate->getHeight() / 2)
	{
		if (onPos)
		{
			crate->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
			crate->getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
			

			timer1++;
			std::cout << timer1;
			if(timer1 == 200)
			{
				isMoving = false;
				onPos = false;
				timer1 = 0;
			}
		}
		else
		{
			//combine both x and y velocity into single magnitude velocity vector along x axis
			crate->getRigidBody()->velocity = glm::vec2(Util::magnitude(crate->getRigidBody()->velocity), 0.0f);
			//Turn acceleration into negative variable due to kinetic friction.
			m_Acceleration = -(m_kineticFriction * m_gravity);

			crate->getRigidBody()->acceleration = { m_Acceleration, 0.0f };
			crate->getRigidBody()->velocity += crate->getRigidBody()->acceleration * dt * (PPM/70);
			crate->getTransform()->position += crate->getRigidBody()->velocity * m_timeScale;
			 if(crate->getRigidBody()->velocity.x <= 0)
				onPos = true;
				
		}
	}
}

void PlayScene::reset()
{

	if(resetVariables)
	{
		time = 0;
		m_Mass = 12.8;
		m_Speed = 25;
	    PPM = 100;
		rampX = 4;
		rampY = 3;

		m_kineticFriction = 0.42;
		m_gravity = 9.8;
		m_fGravity = 9.8;
		 m_normalForce = 9.8;
		m_timeScale = 0.1;
		m_Velocity = 0;
		m_Acceleration = 0;
		isMoving = false;
		onPos = false;
		resetVariables = false;

		crate->getRigidBody()->velocity = glm::vec2(0, 0);
		crate->getRigidBody()->acceleration = glm::vec2(0, 0);

	}
}

void PlayScene::drawTriangle(glm::vec2 v1, glm::vec2(v2), glm::vec2(v3))
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

