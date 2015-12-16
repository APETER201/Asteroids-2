/*
=================
cArcher.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cArcher.h"


void cArcher::render()
{
	
	glPushMatrix();

	glTranslatef(spritePos2D.x, spritePos2D.y, 0.0f);
	glRotatef(spriteRotation, 0.0f, 0.0f, 1.0f);
	glScalef(spriteScaling.x, spriteScaling.y, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextureID); // Binding of GLtexture name 

	glBegin(GL_QUADS);
	glColor3f(255.0f, 255.0f, 255.0f);
	glTexCoord2f(spriteTexCoordData[0].x, spriteTexCoordData[0].y);
	glVertex2f(-(textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[1].x, spriteTexCoordData[1].y);
	glVertex2f((textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[2].x, spriteTexCoordData[2].y);
	glVertex2f((textureWidth / 2), (textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[3].x, spriteTexCoordData[3].y);
	glVertex2f(-(textureWidth / 2), (textureHeight / 2));

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cArcher::update(float deltaTime)
{

	if (m_InputMgr->isKeyDown(VK_UP))
	{
		spriteTranslation = (glm::vec2(30.0f, 10.0f));
	}

	if (m_InputMgr->isKeyDown(VK_DOWN))
	{
		spriteTranslation = -(glm::vec2(30.0f, 10.0f));
	}
	if (m_InputMgr->isKeyDown(int('A')))
	{
		spriteScaling += 0.2f;
	}
	if (m_InputMgr->isKeyDown(int('S')))
	{
		spriteScaling -= 0.2f;
	}

	if (m_InputMgr->isKeyDown(VK_SPACE))
	{
		// Add new arrow sprite to the vector array
		theArcherArrows.push_back(new cArrow);
		int numarrows = theArcherArrows.size() - 1;
		//changed where the arrow appears
		theArcherArrows[numarrows]->setSpritePos(glm::vec2(spritePos2D.x + 40 , spritePos2D.y));
		theArcherArrows[numarrows]->setSpriteTranslation(glm::vec2(2.0f, 2.0f));
		theArcherArrows[numarrows]->setTexture(theGameTextures[4]->getTexture());
		theArcherArrows[numarrows]->setTextureDimensions(theGameTextures[4]->getTWidth(), theGameTextures[4]->getTHeight());
		theArcherArrows[numarrows]->setSpriteCentre();
		theArcherArrows[numarrows]->setArrowVelocity(glm::vec2(0.0f, 0.0f));
		theArcherArrows[numarrows]->setSpriteRotation(getSpriteRotation());
		theArcherArrows[numarrows]->setActive(true);
		theArcherArrows[numarrows]->setMdlRadius();
		// play the arrow release sound
		m_SoundMgr->getSnd("arrowShoot")->playAudio(AL_TRUE);
	}

	if (spriteRotation > 360)
	{
		spriteRotation -= 360.0f;
	}

	glm::vec2 spriteVelocityAdd = glm::vec2(0.0f, 0.0f);
	spriteVelocityAdd.x = (glm::sin(glm::radians(spriteRotation)));
	spriteVelocityAdd.y = -(glm::cos(glm::radians(spriteRotation)));

	spriteVelocityAdd *= spriteTranslation;

	archerVelocity += spriteVelocityAdd;

	spritePos2D += archerVelocity * deltaTime;

	archerVelocity *= 0.95;

	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cArrow*>::iterator arrowIterartor = theArcherArrows.begin(); arrowIterartor != theArcherArrows.end(); ++arrowIterartor)
	{
		(*arrowIterartor)->update(deltaTime);
		for (vector<cSnake*>::iterator snakeIterator = theSnake.begin(); snakeIterator != theSnake.end(); ++snakeIterator)
		{
			if ((*snakeIterator)->collidedWith((*snakeIterator)->getBoundingRect(), (*arrowIterartor)->getBoundingRect()))
			{
				// if a collision set the arrow to false and the snake back to the edge of the screen
				(*snakeIterator)->setSpritePos(glm::vec2(1024, spritePos2D.y));;
				(*arrowIterartor)->setActive(false);

				
				
				
			}
		}
	}
	

	vector<cArrow*>::iterator arrowIterartor = theArcherArrows.begin();
	while (arrowIterartor != theArcherArrows.end())
	{
		if ((*arrowIterartor)->isActive() == false)
		{
			arrowIterartor = theArcherArrows.erase(arrowIterartor);
			// play the sSnake sound.
			m_SoundMgr->getSnd("sSnake")->playAudio(AL_TRUE);
		}
		else
		{
			//(*arrowIterartor)->update(deltaTime);
			(*arrowIterartor)->render();
			++arrowIterartor;
		}
	}
	setBoundingRect(&boundingRect);
}

/*
=================================================================
  Sets the velocity for the archer
=================================================================
*/
void cArcher::setArcherVelocity(glm::vec2 archerVel)
{
	archerVelocity = archerVel;
}
/*
=================================================================
  Gets the archer velocity
=================================================================
*/
glm::vec2 cArcher::getArcherVelocity()
{
	return archerVelocity;
}
