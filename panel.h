#ifndef PANEL_H
#define PANEL_H

#include <sstream>
#include "sdlenv.h"


class Panel{

protected:
	int width, height;
	TTF_Font *font16, *font32;
	SDL_Surface *panel, *titleText, *spaceText;

public:
	Panel(int w, int h, TTF_Font *f16, std::string title); 
	virtual ~Panel(void);
	virtual void draw(SDL_Surface *dest) = 0;

};


#endif
