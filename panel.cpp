#include "panel.h"

Panel::Panel(int w, int h, TTF_Font *f16, std::string title) : 	width(w),
								height(h),
								font16(f16)
{
								

	int i;

	panel = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, BPP, screen->format->Rmask, \
				     screen->format->Gmask, screen->format->Bmask, screen->format->Amask);

	titleText = TTF_RenderText_Blended(font16, title.c_str(), green);
	spaceText = TTF_RenderText_Blended(font16, "Press <SPACE> to continue", green);

	for(i=0; i < panel->h; ++i){
		drawPixel(panel, 0, i, 0xFF, 0xFF, 0xFF);
		drawPixel(panel, panel->w-1, i, 0xFF, 0xFF, 0xFF);
	}

	for(i=0; i < panel->w; ++i){
		drawPixel(panel, i, 0, 0xFF, 0xFF, 0xFF);
		drawPixel(panel, i, panel->h-1, 0xFF, 0xFF, 0xFF);
	}

	for(i=3; i < panel->h-3; ++i){
		drawPixel(panel, 3, i, 0xFF, 0xFF, 0xFF);
		drawPixel(panel, panel->w-4, i, 0xFF, 0xFF, 0xFF);
	}

	for(i=3; i < panel->w-3; ++i){
		drawPixel(panel, i, 3, 0xFF, 0xFF, 0xFF);
		drawPixel(panel, i, panel->h-4, 0xFF, 0xFF, 0xFF);
	}

	blitSurface(width/2-titleText->w/2, 6, titleText, panel);
	blitSurface(width/2-spaceText->w/2, height-2*spaceText->h, spaceText, panel);
							
}


Panel::~Panel(void){

	SDL_FreeSurface(titleText);
	SDL_FreeSurface(spaceText);

}

