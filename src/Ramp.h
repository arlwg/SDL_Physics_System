#pragma once
#include "TextureManager.h"
#include <glm/vec4.hpp>

#include "Agent.h"

class Ramp final : public Agent
{
public:
	Ramp();
	~Ramp();
	void update() override;
	void draw() override;
	void clean() override;
	short type = 0;
};