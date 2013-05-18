#include "about.h"

AboutPanel::AboutPanel(int w, int h, TTF_Font *f16, std::string title) : Panel(w, h, f16, title)
{

	creditText.push_back(TTF_RenderText_Blended(font16, "A toroidal version", white));
	creditText.push_back(TTF_RenderText_Blended(font16, "of", white));
	creditText.push_back(TTF_RenderText_Blended(font16, "Conway's Game of Life", white));
	creditText.push_back(TTF_RenderText_Blended(font16, " ", white));
	creditText.push_back(TTF_RenderText_Blended(font16, "by", white));
	creditText.push_back(TTF_RenderText_Blended(font16, "Paul Slavin", white));
	creditText.push_back(TTF_RenderText_Blended(font16, "<slavinp@cs.man.ac.uk>", white));


}



AboutPanel::~AboutPanel(void){

//	SDL_FreeSurface(creditText);

}


void AboutPanel::draw(SDL_Surface *dest)
{
	int x = dest->w/2 - width/2;
	int y = dest->h/2 - height/2;
	int line = 0;

	std::vector<SDL_Surface *>::iterator it;

	for(it = creditText.begin(); it != creditText.end(); it++){
		blitSurface(panel->w/2-((*it)->w)/2, 5*titleText->h+line*((*it)->h), *it, panel);
		line++;
	}


//	blitSurface(panel->w/2-creditText->w/2, 3*titleText->h, creditText, panel);

	blitSurface(x, y, panel, dest);

	SDL_Flip(dest);

}
