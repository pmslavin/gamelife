#include "statusbar.h"


StatusBar::StatusBar(int w, int h, TTF_Font *font, Grid *g) : width(w), height(h), font16(font), grid(g) {

	bar = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, width, height, BPP, \
					screen->format->Rmask, screen->format->Gmask, \
					screen->format->Bmask, screen->format->Amask);

	genText = TTF_RenderText_Blended(font16, "Generation: ", white);
	popText = TTF_RenderText_Blended(font16, "Population: ", white);
	tickText = TTF_RenderText_Blended(font16, "Tick: ", white);
	pausedText = TTF_RenderText_Blended(font16, "PAUSED", green);
	zoomText = TTF_RenderText_Blended(font16, "Zoom: ", white);

}


void StatusBar::paint(SDL_Surface *dest, bool isPaused, Uint32 delay){

	std::ostringstream genss, popss, tickss, zoomss;

	genss << grid->getGen();
	popss << grid->getPop();
//	tickss << (delay > 0 ? delay/1000 : 20) << " ms";
	tickss << delay/1000 << " ms";
	zoomss << grid->getZoom();

	std::string genNum = genss.str();
	std::string popNum = popss.str();
	std::string tickNum = tickss.str();
	std::string zoomNum = zoomss.str();

	SDL_Surface *genNumText = TTF_RenderText_Blended(font16, genNum.c_str(), white);
	SDL_Surface *popNumText = TTF_RenderText_Blended(font16, popNum.c_str(), white);
	SDL_Surface *tickNumText = TTF_RenderText_Blended(font16, tickNum.c_str(), white);
	SDL_Surface *zoomNumText = TTF_RenderText_Blended(font16, zoomNum.c_str(), white);

	SDL_FillRect(bar, &bar->clip_rect, SDL_MapRGB(bar->format, 0, 0, 0));

	blitSurface(0, bar->h-genText->h, genText, bar);
	blitSurface(genText->w, bar->h-genText->h, genNumText, bar);
	blitSurface(2*genText->w, bar->h-genText->h, popText, bar);
	blitSurface(2*genText->w+popText->w, bar->h-genText->h, popNumText, bar);
	blitSurface(2*genText->w+2*popText->w, bar->h-genText->h, tickText, bar);
	blitSurface(2*genText->w+2*popText->w+tickText->w, bar->h-genText->h, tickNumText, bar);
	blitSurface(2*genText->w+2*popText->w+4*tickText->w, bar->h-genText->h, zoomText, bar);
	blitSurface(2*genText->w+2*popText->w+4*tickText->w+zoomText->w, bar->h-genText->h, zoomNumText, bar);


	if(isPaused)
		blitSurface(bar->w-pausedText->w-2, bar->h-genText->h, pausedText, bar);

	blitSurface(0, dest->h-bar->h, bar, dest);

	SDL_FreeSurface(genNumText);
	SDL_FreeSurface(popNumText);
	SDL_FreeSurface(tickNumText);
	SDL_FreeSurface(zoomNumText);

}



StatusBar::~StatusBar(void){

	SDL_FreeSurface(genText);
	SDL_FreeSurface(popText);
	SDL_FreeSurface(tickText);

	SDL_FreeSurface(bar);
}
