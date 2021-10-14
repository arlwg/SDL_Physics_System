#pragma once
#include "TextureManager.h"
#include <glm/vec4.hpp>

#include "Agent.h"

class Crate final : public Agent
{
public:
	Crate();
	~Crate();
	void update() override;
	void draw() override;
	void clean() override;
	short type = 0;
};