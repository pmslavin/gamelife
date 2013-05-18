#include "help.h"

HelpPanel::HelpPanel(int w, int h, TTF_Font *f, std::string title) : Panel(w, h, f, title)
{

	keymap["0Pause"] = "P";
	keymap["1Decrease delay"] = "<";
	keymap["2Increase delay"] = ">";
	keymap["3Reset world"] = "R";
	keymap["4Show/Hide status bar"] = "B";
	keymap["5 "] = " ";
	keymap["6Zoom in"] = "Z";
	keymap["7Zoom out"] = "X";
	keymap["8Pan while zoomed"] = "Cursor Keys";
	keymap["9Reset zoom"] = "L";
	keymap["a "] = " ";
	keymap["bAbout Game of Life"] = "A";
	keymap["cQuit"] = "Q";


}


HelpPanel::~HelpPanel(void){

	

}



void HelpPanel::draw(SDL_Surface *dest){

	int x = dest->w/2 - width/2;
	int y = dest->h/2 - height/2;
	int line = 0;

	std::map<std::string, std::string>::iterator i;

	for(i = keymap.begin(); i != keymap.end(); ++i){

		SDL_Surface *keyText = TTF_RenderText_Blended(font16, ((i->first).substr(1,i->first.length())).c_str(), white);
		SDL_Surface *keyVal = TTF_RenderText_Blended(font16, (i->second).c_str(), green);

		blitSurface(width/10, 3*titleText->h+line*keyText->h, keyText, panel);
		blitSurface(width-width/4, 3*titleText->h+line*keyText->h, keyVal, panel);

		line++;

		SDL_FreeSurface(keyText);
		SDL_FreeSurface(keyVal);
	
	}

	blitSurface(x, y, panel, dest);

	SDL_Flip(dest);

}
