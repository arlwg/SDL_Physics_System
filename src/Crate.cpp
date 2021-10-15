#include "Crate.h"

Crate::Crate()
{
	TextureManager::Instance().load("../Assets/textures/crate.png", "crate");
	auto size = TextureManager::Instance().getTextureSize("crate");

	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(AGENT);

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right

	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));

}

Crate::~Crate() = default;

void Crate::update()
{
}

void Crate::draw()
{
	//// alias for x and y
	//const auto x = getTransform()->position.x;
	//const auto y = getTransform()->position.y;
	TextureManager::Instance().draw("crate", getTransform()->position.x,  getTransform()->position.y, getCurrentHeading(), 255, true);
}

void Crate::clean()
{
}
