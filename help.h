#ifndef HELP_H
#define HELP_H

#include <map>
#include "panel.h"


class HelpPanel : public Panel{

private:
	std::map<std::string, std::string> keymap;


public:
	HelpPanel(int w, int h, TTF_Font *f, std::string title);
	virtual ~HelpPanel(void);
	virtual void draw(SDL_Surface *dest);

};



#endif
