/*
==========================================================================
cFont.cpp
==========================================================================
*/
#pragma warning ( disable : 4996 )
#include "cFont.h"
#include <sstream>
using namespace std;
std::stringstream ss;
/*
==========================================================================
Default Constructor
==========================================================================
*/
cFont::cFont()
{
	theFont = NULL;
	
}
/*
==========================================================================
Overloaded constructor
==========================================================================
*/
cFont::cFont(LPCSTR fontFileName, int fontSize)
{
	theFont = new FTTextureFont(fontFileName);

	if (theFont == NULL)
	{
		MessageBox(NULL, "Unable to create the required Font!", "An error occurred", MB_ICONERROR | MB_OK);
		delete theFont;
	}

	if (!theFont->FaceSize(fontSize))
	{
		MessageBox(NULL, "Unable to set desired size for Font!", "An error occurred", MB_ICONERROR | MB_OK);
		delete theFont;
	}
}

/*
==========================================================================
Destructor
==========================================================================
*/
cFont::~cFont()
{
	delete theFont;
}
/*
==========================================================================
get the pointer to the font
==========================================================================
*/
FTFont* cFont::getFont()
{
	return theFont;
}

/*
==========================================================================
Render the text using the desired font
==========================================================================
*/
void cFont::printText(LPCSTR text, FTPoint textPos)
{
	glPushMatrix();

	glTranslatef(0.0f, 25.0f, 0.0f);
	glScalef(1.0f, -1.0f, 0.0f);

	glColor3f(255.0f, 255.0f, 255.0f);
	theFont->Render(text,  -1, textPos);

	glPopMatrix();
}
