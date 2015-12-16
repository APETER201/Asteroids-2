/*
=================
cSnake.h
- Header file for class definition - SPECIFICATION
- Header file for the Archer class which is a child of cSprite class
=================
*/
#ifndef _CSNAKE_H
#define _CSNAKE_H
#include "cSprite.h"

class cSnake : public cSprite
{
private:
	glm::vec2 snakeVelocity = glm::vec2(0.0f, 0.0f);

public:
	void render();		// Default render function
	void update(float deltaTime);		// Archer update method
	void setSnakeVelocity(glm::vec2 snakeVel);   // Sets the velocity for the Archer
	glm::vec2 getSnakeVelocity();				 // Gets the Archer velocity
	void renderCollisionBox();				// Use this function to show the collision box
};
#endif