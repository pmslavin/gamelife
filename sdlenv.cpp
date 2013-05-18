#include <string>
#include "sdlenv.h"

SDL_Surface *screen;
SDL_Color white = {0xFF, 0xFF, 0xFF, 0};
SDL_Color green = {0, 0xFF, 0, 0};

void initialise(std::string wmtitle){

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)){
		fprintf(stderr, "Unable to initialise SDL.\n");
		exit(1);
	}

	screen = SDL_SetVideoMode(SCREENW, SCREENH, BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);

	if(!screen){
		fprintf(stderr, "Unable to set video mode %dx%x:%d.\n", SCREENW, SCREENH, BPP);
		exit(1);
	}


	//SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_SetCaption(wmtitle.c_str(), NULL);

	TTF_Init();

}


void tidy(void){

	SDL_FreeSurface(screen);
	SDL_Quit();

}

/*
SDL_Surface *loadImage(const std::string path){

	SDL_Surface *rawimage = IMG_Load(path.c_str());

	if(!rawimage){
		fprintf(stderr, "Unable to load image %s\n", path.c_str() );
		exit(1);
	}

	SDL_Surface *optimage = SDL_DisplayFormat(rawimage);
	SDL_SetColorKey(optimage, SDL_SRCCOLORKEY, 0);
	SDL_FreeSurface(rawimage);

	return optimage;

}


SDL_Surface *loadImageResource(unsigned char res[], unsigned int len){

	SDL_RWops *rw = SDL_RWFromMem((void *)res, len);
	SDL_Surface *rawImage = IMG_LoadPNG_RW(rw);

	if(!rawImage){
		fprintf(stderr, "Unable to load image.\n" );
		exit(1);
	}

	SDL_Surface *optimage = SDL_DisplayFormat(rawImage);
	SDL_SetColorKey(optimage, SDL_SRCCOLORKEY, 0);
	SDL_FreeSurface(rawImage);
	SDL_FreeRW(rw);

	return optimage;

}


Mix_Chunk *loadChunkResource(unsigned char res[], unsigned int len){

	SDL_RWops *rw = SDL_RWFromMem((void *)res, len);
	Mix_Chunk *chunk = Mix_LoadWAV_RW(rw, 0);

	if(!chunk){
		fprintf(stderr, "Unable to load audio resource.\n");
		exit(1);
	}

	SDL_FreeRW(rw);

	return chunk;
}


Mix_Music *loadMusicResource(unsigned char res[], unsigned int len){

	SDL_RWops *rw = SDL_RWFromMem((void *)res, len);
	Mix_Music *music = Mix_LoadMUS_RW(rw);

	if(!music){
		fprintf(stderr, "Unable to load music resource.\n");
		exit(1);
	}

//	SDL_FreeRW(rw);

	return music;
}
*/

void blitSurface(int x, int y, SDL_Surface *src, SDL_Surface *dest, SDL_Rect *clip){

	SDL_Rect offset = {x, y, 0, 0};

	SDL_BlitSurface(src, clip, dest, &offset);

}

void drawPixel(SDL_Surface *dest, const int x, const int y, Uint8 R, Uint8 G, Uint8 B){

	Uint32 colour = SDL_MapRGB(dest->format, R, G, B);

	if( SDL_MUSTLOCK(dest) ){
		if( SDL_LockSurface(dest) ){
			return;
		}
	}

	Uint32 *bufp = (Uint32 *)dest->pixels + dest->pitch/4*y + x;
	*bufp = colour;

	SDL_UnlockSurface(dest);
}
