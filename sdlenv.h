#ifndef SDLENV_H
#define SDLENV_H

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"


const int SCREENW = 1024;
const int SCREENH = 768;
const int BPP = 32;

extern SDL_Surface *screen;
extern SDL_Color white, green;

void initialise(std::string wmtitle);
void tidy(void);
/*
SDL_Surface *loadImage(const std::string path);
SDL_Surface *loadImageResource(unsigned char res[], unsigned int len);
Mix_Chunk *loadChunkResource(unsigned char res[], unsigned int len);
Mix_Music *loadMusicResource(unsigned char res[], unsigned int len);
*/
void blitSurface(int x, int y, SDL_Surface *src, SDL_Surface *dest, SDL_Rect *clip = NULL);
void drawPixel(SDL_Surface *dest, const int x, const int y, Uint8 R, Uint8 G, Uint8 B);


#endif
