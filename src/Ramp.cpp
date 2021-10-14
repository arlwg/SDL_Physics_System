#include "Ramp.h"

Ramp::Ramp()
{
	TextureManager::Instance().load("../Assets/textures/ramp.png", "ramp");
	auto size = TextureManager::Instance().getTextureSize("ramp");

	setWidth(size.x);
	setHeight(size.y);


	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(AGENT);

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right

	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));
}

Ramp::~Ramp() = default;

void Ramp::update()
{

}

void Ramp::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	TextureManager::Instance().draw("ramp", x, y, getCurrentHeading(), 255, true);
}

void Ramp::clean()
{
}
