#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <sstream>
#include "sdlenv.h"
#include "grid.h"

class StatusBar {

private:
	int width, height;
	TTF_Font *font16;
	SDL_Surface *bar, *genText, *gen, *popText, *pop, *tickText, *pausedText, *zoomText;
	Grid *grid;

public:
	StatusBar(int w, int h, TTF_Font *f, Grid *g);
	~StatusBar(void);
	void paint(SDL_Surface *dest, bool, Uint32);


};


#endif
