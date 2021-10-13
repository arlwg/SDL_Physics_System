#pragma once
#include "TextureManager.h"
#include <glm/vec4.hpp>

#include "Agent.h"

class StormTrooper final : public Agent
{
public:
	StormTrooper();
	~StormTrooper();

	void update() override;
	void draw() override;
	void clean() override;
};