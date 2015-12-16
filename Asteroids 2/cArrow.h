/*
=================
cArrow.h
- Header file for class definition - SPECIFICATION
- Header file for the arrow class which is a child of cSprite class
=================
*/
#ifndef _CARROW_H
#define _CARROW_H
#include "cSprite.h"


class cArrow : public cSprite
{
private:
	glm::vec2 ArrowVelocity = glm::vec2(0.0f, 0.0f);

public:
	//cArrow(vector<cTexture*> tileTextList)

	void render();		// Default render function
	void update(float deltaTime);		// arrow update method
	void setArrowVelocity(glm::vec2 arrowVel);   // Sets the velocity for the arrow
	glm::vec2 getArrowVelocity();				 // Gets the arrow velocity
	void renderCollisionBox();				// Use this function to show the collision box
};
#endif