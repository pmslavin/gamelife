#include "cell.h"
#include "grid.h"
#include "statusbar.h"
#include "sdlenv.h"
#include "help.h"
#include "about.h"

const int ROWS = 768;
const int COLS = 1024;
const int FPS = 100;

SDL_Event event;
TTF_Font *font16;

int main(int argc, char *argv[]){

	Uint32 renderstart = 0;
	Uint32 stepstart = 0;
	Uint32 delay = 250000;
	bool paused = false, baractive = true;

	Grid grid(ROWS, COLS, 0.15);
	initialise("Game of Life");

	font16 = TTF_OpenFont("Prototype.ttf", 16);
	StatusBar bar(screen->w, 20, font16, &grid);

	for(int i=0; i<50000; ++i){
		renderstart = SDL_GetTicks();

		while(SDL_PollEvent(&event)){
			grid.inputHandler(event);
			if(event.type == SDL_KEYDOWN){
				if(event.key.keysym.sym == SDLK_PERIOD){
					delay += 50000;
				}
				if(event.key.keysym.sym == SDLK_COMMA){
					if(delay >= 50000)
						delay -= 50000;
				}
/*				if(event.key.keysym.sym == SDLK_UP){
					grid.decPortal_y();
				}
				if(event.key.keysym.sym == SDLK_DOWN){
					grid.incPortal_y();
				}
				if(event.key.keysym.sym == SDLK_LEFT){
					grid.decPortal_x();
				}
				if(event.key.keysym.sym == SDLK_RIGHT){
					grid.incPortal_x();
				}
*/
				if(event.key.keysym.sym == SDLK_p){
					paused = !paused;
				}
				if(event.key.keysym.sym == SDLK_b){
					baractive = !baractive;
				}
				if(event.key.keysym.sym == SDLK_r){
					grid.reset(0.15);
				}
				if(event.key.keysym.sym == SDLK_z){
					grid.zoomIn();
				}
				if(event.key.keysym.sym == SDLK_x){
					grid.zoomOut();
				}
				if(event.key.keysym.sym == SDLK_l){
					grid.zoomReset();
				}
				if(event.key.keysym.sym == SDLK_q){
					exit(0);
				}
				if(event.key.keysym.sym == SDLK_h){
					HelpPanel *hp = new HelpPanel(480, 400, font16, "Help Panel");
					bool wasPaused = paused;
					if(!wasPaused)
						paused = true;

					hp->draw(screen);
					while(SDL_WaitEvent(&event)){
						if(event.type == SDL_KEYDOWN){
							if(event.key.keysym.sym == SDLK_SPACE)
								break;
						}
					}
					delete hp;

					if(!wasPaused)
						paused = false;

				}
				if(event.key.keysym.sym == SDLK_a){
					AboutPanel *ap = new AboutPanel(480, 400, font16, "Game of Life");
					bool wasPaused = paused;
					if(!wasPaused)
						paused = true;

					ap->draw(screen);
					while(SDL_WaitEvent(&event)){
						if(event.type == SDL_KEYDOWN){
							if(event.key.keysym.sym == SDLK_SPACE)
								break;
						}
					}

					delete ap;

					if(!wasPaused)
						paused = false;
				}

				if(event.key.keysym.sym == SDLK_w){
					//grid.pstep(4);
					grid.writeBMP("gol-01.bmp");
				}
				if(event.key.keysym.sym == SDLK_f){
					//grid.readBMP("gol-01.bmp");
					//grid.readBMP("glider.bmp");
					//grid.readBMP("rabbits.bmp");
					grid.readBMP("gun.bmp");
				}

			}
		}

		if(!paused && ((SDL_GetTicks() - stepstart) > delay/1000)) {
			stepstart = SDL_GetTicks();
			//grid.step();
			grid.pstep(4);
		}
		
//		SDL_LockSurface(screen);
		grid.update();
		grid.draw(screen);
		
		
		if(baractive)
			bar.paint(screen, paused, delay);
		
//		SDL_UnlockSurface(screen);
		SDL_Flip(screen);
//		fprintf(stderr, "Delay: %d\n", delay);

		if( (SDL_GetTicks() - renderstart) < 1000/FPS  ){
//			fprintf(stderr, "[%d] In render delay.\n", i);
//			fprintf(stderr, "---- Margin %d vs. %d ---- In delay for %d", \
					(SDL_GetTicks() - renderstart) , (1000/FPS - delay/1000), \
					(1000/FPS - (SDL_GetTicks() - renderstart) + delay/1000 ));
			SDL_Delay(1000/FPS - (SDL_GetTicks() - renderstart) );
		}

		
	}

	return 0;

}

