#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "snakesGame.h"
#include "cButton.h"
#include <sstream>


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{
    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;
	int score = 0;
	int snaky = 0;
	int timer = 0;
	//std::stringstream ss;
	

    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
    windowOGL theOGLWnd;
	
    //Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	 //Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\Snake.png", "Images\\Snake.png", "Images\\Snake.png", "Images\\Snake.png", "Images\\Arrow.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// Create Vector array for backgrounds
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images/Grassland1024x768.png", "Images/darksnake.png", "Images/endscreenBkg.png" };
	for (int tCount = 0; tCount < 3; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}

	cTexture transSprite;
	transSprite.createTexture("Images/blank.png");

	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgList[0]->getTexture());
	spriteBkgd.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	cBkGround spriteStartBkgd;
	spriteStartBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteStartBkgd.setTexture(textureBkgList[1]->getTexture());
	spriteStartBkgd.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	cBkGround spriteEndBkgd;
	spriteEndBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteEndBkgd.setTexture(textureBkgList[2]->getTexture());
	spriteEndBkgd.setTextureDimensions(textureBkgList[2]->getTWidth(), textureBkgList[2]->getTHeight());

	// Create vector array of button textures
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/Buttons/exitBtn.png", "Images/Buttons/instructionsBtn.png", "Images/Buttons/loadBtn.png", "Images/Buttons/playBtn.png", "Images/Buttons/saveBtn.png", "Images/Buttons/settingsBtn.png" };
	for (int tCount = 0; tCount < 6; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[0]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[3]->getTexture());
	playButton.setTextureDimensions(btnTextureList[3]->getTWidth(), btnTextureList[3]->getTHeight());

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/Rainforest.wav", "Audio/Arrow.wav", "Audio/snake.wav" };

	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("arrowShoot", gameSounds[1]);
	theSoundMgr->add("sSnake", gameSounds[2]);

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);
	
	// create string for score
	string strScore = "";

	// Create vector array of textures for snakes and put them on screen


	// Create the archer and give it it's image
	cTexture archerTxt;
	archerTxt.createTexture("Images\\Archer.png");
	cArcher archerSprite;
	archerSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	archerSprite.setSpritePos(glm::vec2(50.0f, 380.0f));
	archerSprite.setTexture(archerTxt.getTexture());
	archerSprite.setTextureDimensions(archerTxt.getTWidth(), archerTxt.getTHeight());
	archerSprite.setSpriteCentre();
	archerSprite.setArcherVelocity(glm::vec2(0.0f, 0.0f));


	// include an exit button

	cFileHandler theFile("Data/usermap.dat");
	string mapData;

	string outputMsg;
	string strMsg[] = { "Shoot the snakes", "Don't get hit", "Snakes and Arrows", "Thanks for playing!", "Your score was:" };



	gameState theGameState = MENU;
	btnTypes theBtnType = EXIT;
	
	// Attach sound manager to archer sprite
	archerSprite.attachSoundMgr(theSoundMgr);

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//spriteBkgd.render();

		

		switch (theGameState)
		{
		case MENU:
			spriteStartBkgd.render();

			playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
			playButton.render();
			exitButton.render();

			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update();


			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
			outputMsg = strMsg[0];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
			outputMsg = strMsg[1];
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}

			break;
		case PLAYING:

			spriteBkgd.render();

		
			archerSprite.update(elapsedTime);
			{
				vector<cSnake*>::iterator snakeIterator = theSnake.begin();
				while (snakeIterator != theSnake.end())
				{
					if ((*snakeIterator)->isActive() == false)
					{
						snakeIterator = theSnake.erase(snakeIterator);
					    snaky--;
						score++;
					}
	//				else if (archerSprite.collidedWith(archerSprite.getBoundingRect(), (*snakeIterator)->getBoundingRect()))
	//				{
	//					theGameState = exitButton.update(theGameState, END);
	//					
	//				}
					else
					{
						(*snakeIterator)->update(elapsedTime);
						(*snakeIterator)->render();
						
						++snakeIterator;
					}
				}
			}
			
			timer++;

			if (timer > 30)
			{
				theSnake.push_back(new cSnake);
				theSnake[snaky]->setSpritePos(glm::vec2(1024,rand() % windowHeight));
				theSnake[snaky]->setSpriteTranslation(glm::vec2((rand() % 4 + 1), (rand() % 4 + 1)));
				int randSnake = rand() % 4;
				theSnake[snaky]->setTexture(theGameTextures[randSnake]->getTexture());
				theSnake[snaky]->setTextureDimensions(theGameTextures[randSnake]->getTWidth(), theGameTextures[randSnake]->getTHeight());
				theSnake[snaky]->setSpriteCentre();
				theSnake[snaky]->setSnakeVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
				theSnake[snaky]->setActive(true);
				theSnake[snaky]->setMdlRadius();

				snaky++;

				timer = 0;
			}
			
			exitButton.setSpritePos(glm::vec2(740.0f, 575.0f));
			exitButton.render();
			theGameState = exitButton.update(theGameState, END);

			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));

			strScore = "Score: " + std::to_string(score);
			theFontMgr->getFont("Space")->printText(strScore.c_str(), FTPoint(0.0f, 1.0f, 0.0f));

			archerSprite.render();

			break;
		case END:
			spriteEndBkgd.render();

			playButton.setClicked(false);
			exitButton.setClicked(false);

			playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
			playButton.render();
			exitButton.render();

			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update();

			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, -15, 0.0f));
			outputMsg = strMsg[3];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, -100, 0.0f));
			outputMsg = strMsg[4];
			theFontMgr->getFont("Space")->printText(strScore.c_str(), FTPoint(100, -150, 0.0f));
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}

			break;
		}


		//ss << name << score, FTPoint(0.0f, 1.0f, 0.0f);
	

		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
