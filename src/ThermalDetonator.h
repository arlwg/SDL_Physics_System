#pragma once
#include "TextureManager.h"
#include <glm/vec4.hpp>

#include "Agent.h"

class ThermalDetonator final : public Agent
{
public:
	ThermalDetonator();
	~ThermalDetonator();

	void update() override;
	void draw() override;
	void clean() override;
};