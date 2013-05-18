#ifndef ABOUT_H
#define ABOUT_H

#include <vector>
#include "panel.h"


class AboutPanel : public Panel{

private:
	std::vector<SDL_Surface *> creditText;


public:
	AboutPanel(int w, int h, TTF_Font *f16, std::string title);
	virtual ~AboutPanel(void);
	virtual void draw(SDL_Surface *dest);

};


#endif
