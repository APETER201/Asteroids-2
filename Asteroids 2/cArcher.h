/*
=================
cArcher.h
- Header file for class definition - SPECIFICATION
- Header file for the Archer class which is a child of cSprite class
=================
*/
#ifndef _CARCHER_H
#define _CARCHER_H
#include "cSprite.h"
#include "snakesGame.h"


class cArcher : public cSprite
{
private:
	glm::vec2 archerVelocity = glm::vec2(0.0f, 0.0f);
public:
	void render();		// Default render function
	void update(float deltaTime);		// archer update method
	void setArcherVelocity(glm::vec2 archerVel);   // Sets the velocity for the archer
	glm::vec2 getArcherVelocity();				 // Gets the Archer velocity

};
#endif