#pragma once
#pragma once
#include "TextureManager.h"
#include <glm/vec4.hpp>

#include "Agent.h"

class Background final : public Agent
{
public:
	Background();
	~Background();
	
	void update() override;
	void draw() override;
	void clean() override;
	short type = 0;
};

