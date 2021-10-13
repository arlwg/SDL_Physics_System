#include "ThermalDetonator.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"

ThermalDetonator::ThermalDetonator()
{
	TextureManager::Instance().load("../Assets/textures/bomb.png", "bomb");
	getTransform()->position = glm::vec2(0.0f,0.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0,9.8);
	setCurrentHeading(Game::Instance().angle);
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	setType(BOMB);
	
}

ThermalDetonator::~ThermalDetonator()
{
}

void ThermalDetonator::update()
{
	setCurrentHeading(Game::Instance().angle);
	//need to use current direction later
	
}

void ThermalDetonator::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	//const std::string & id, const int x, const int y, const double angle, const int alpha, const bool centered, const SDL_RendererFlip flip
	TextureManager::Instance().draw("bomb", x, y, getCurrentHeading(), 255,true);
}

void ThermalDetonator::clean()
{
}
