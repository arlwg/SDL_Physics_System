#pragma once
#include "TextureManager.h"
#include <glm/vec4.hpp>

#include "Agent.h"

class Wookie final : public Agent
{
public:
	Wookie();
	~Wookie();

	void update() override;
	void draw() override;
	void clean() override;
};