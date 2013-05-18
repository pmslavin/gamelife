#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "pthread.h"
#include "threadparam.h"
#include "threadset.h"
#include "grid.h"


void *launchThread(void *obj){

	ThreadSet *ts = reinterpret_cast<ThreadSet *>(obj);
	return ts->launch();

}



Grid::Grid(int r, int c, const double prob) : rows(r),
       					cols(c),
				       	population(0),
					generation(1),
					zoom(1),
					initProbability(1-prob) {

	board = new Cell *[rows];

	for(int i=0; i < rows; ++i)
		board[i] = new Cell[cols];

	portal_x = 0;
	portal_y = 0;
	pvel_x = 0;
	pvel_y = 0;

	initialise();

}


void Grid::initialise(void){

	srand(time(NULL));

	for(int i=0; i<rows; ++i){
		for(int j=0; j<cols; ++j){
			if(rand() < RAND_MAX*initProbability){
				board[i][j].state = board[i][j].laststate = 0;
			}else{
				board[i][j].state = board[i][j].laststate = 1;
				population++;
			}
		}
	}

}


void Grid::clearBoard(void){

	for(int i=0; i<rows; ++i)
		for(int j=0; j<cols; ++j){
			board[i][j].age = board[i][j].state = board[i][j].laststate = 0;
		}

}



Grid::~Grid(void) {

	for(int i=0; i < rows; ++i)
		delete [] board[i];


	delete [] board;

}



void Grid::report(void){

	for(int i=0; i<rows; ++i){
		for(int j=0; j<cols; ++j){
			fprintf(stderr, "Grid[%d][%d]: %s\n", i, j, board[i][j].state == 1 ? "ALIVE" : "DEAD" );
		}
	}

	fprintf(stderr, "\n");

}


void Grid::step(void){

	int r, c, neighbours;
	int col_left, col_right, row_up, row_down;

	Cell *topleft, *top, *topright, *right, *bottomright, *bottom, *bottomleft, *left;

	for(r=0; r<rows; ++r){
		for(c=0; c<cols; ++c){

			// Toroidal joining
			if(r==0)
				row_up = rows-1;
		       	else
				row_up = r-1;

			if(r==(rows-1))
				row_down = 0;
			else
				row_down = r+1;

			if(c==0)
				col_left = cols-1;
			else
				col_left = c-1;

			if(c==(cols-1))
				col_right = 0;
			else
				col_right = c+1;


			neighbours = 0;

			topleft = &board[row_up][col_left];
			top = &board[row_up][c];
			topright = &board[row_up][col_right];
			right = &board[r][col_right];
			bottomright = &board[row_down][col_right];
			bottom = &board[row_down][c];
			bottomleft = &board[row_down][col_left];
			left = &board[r][col_left];

			neighbours += topleft->laststate;
			neighbours += top->laststate;
			neighbours += topright->laststate;
			if(c==cols-1)
				neighbours += right->laststate;
			else
				neighbours += right->state;

			neighbours += bottomright->state;
			neighbours += bottom->state;
			neighbours += bottomleft->state;
			if(c==0)
				neighbours += left->state;
			else
				neighbours += left->laststate;

			board[r][c].laststate = board[r][c].state;


			if((neighbours < 2) || (neighbours > 3)){ // Death lonliness or overcrowding
				board[r][c].state = 0;
				board[r][c].age = 0;

				if(board[r][c].laststate == 1)		--population;
			
			}else if(neighbours == 3){	//Birth or continue

				board[r][c].state = 1;
				board[r][c].age++;
			
				if(board[r][c].laststate == 0)		++population;

			}else if(neighbours == 2){	//Stasis
				if(board[r][c].state == 1)		board[r][c].age++;
			}


		}
	}

	generation++;

}


void Grid::draw(SDL_Surface *dest){

	int r, c, i, j, x, y, age;
	Uint8 R,G,B;

	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));

	int row_offset = rows/(2*zoom);
	int col_offset = cols/(2*zoom);

	int row_offset_up = rows/2 - row_offset + portal_y;
	int row_offset_down = rows/2 + row_offset + portal_y;
	int col_offset_left = cols/2 - col_offset + portal_x;
	int col_offset_right = cols/2 + col_offset + portal_x;

	if(row_offset_up < 0)
		row_offset_up = 0;

	if(row_offset_down > rows)
		row_offset_down = rows;

	if(col_offset_left < 0)
		col_offset_left = 0;

	if(col_offset_right > cols)
		col_offset_right = cols;

	for(r=row_offset_up ; r<row_offset_down; ++r){
		for(c=col_offset_left; c<col_offset_right ; ++c){
			if(board[r][c].state == 1){
				R = G = B = 0xFF;
				age = board[r][c].age;


				if(age > 2 && age <= 6){
					R = 0x74; G = 0xFF; B = 0x74;
				}

				if(age > 6 && age <= 9){
					R = 0; G = 0xFF; B = 0;
				}

				if(age > 9 && age <= 13){
					R = 0xFF; G = 0xFF; B = 0;
				}

				if(age > 13 && age <= 19){
					R = 0x7F; G = 0; B = 0x7F;
				}

				if(age > 19){
					R = 0; G = 0; B = 0xFF;
				}

//				drawPixel(dest, c, r, R, G, B);

				for(i=0; i<zoom; ++i){
					for(j=0; j<zoom; ++j){
						x = c*zoom+i-zoom*col_offset_left;
						y = r*zoom+j-zoom*row_offset_up;

						if((x > 0) && (x < screen->w) && (y > 0) && (y < screen->h))
							drawPixel(dest, x, y, R, G, B);
					}
				}

			}
		
		}
	
	}


//	SDL_Flip(dest);
}


void Grid::reset(const double prob){ 
	
	initProbability = 1-prob;
	population = 0;
	generation = 1;
	initialise();

}



void Grid::zoomOut(void){


	if(zoom>1)
		--zoom;
	
	if(zoom == 1){
		portal_x = portal_y = 0;
	}else{
		int row_offset_down = rows/2 + rows/(2*zoom) + portal_y;
		int col_offset_right = cols/2 + cols/(2*zoom) + portal_x;
		if(row_offset_down*zoom < screen->h){
			portal_y += (screen->h - row_offset_down*zoom) - 1;
//			fprintf(stderr, "p_y adjusted.\n");
		}
		if(col_offset_right*zoom < screen->w){
			portal_x += (screen->w - col_offset_right*zoom) - 1;
//			fprintf(stderr, "p_x adjusted.\n");
		}
	}

//	fprintf(stderr, "p_x: %d p_y: %d\n", portal_x, portal_y);
}


void Grid::decPortal_x(void){

	if(zoom ==1)
		return;

	portal_x -= 5;

	int col_offset_left = cols/2 - cols/(2*zoom) + portal_x;

	if(col_offset_left < 0)
		portal_x += 5;
}


void Grid::incPortal_x(void){

	if(zoom ==1)
		return;

	portal_x += 5;

	int col_offset_right = cols/2 + cols/(2*zoom) + portal_x;

	if(col_offset_right > screen->w)
		portal_x -=5;
}


void Grid::decPortal_y(void){

	if(zoom ==1)
		return;

	portal_y -= 5;

	int row_offset_up = rows/2 - rows/(2*zoom) + portal_y;

	if(row_offset_up < 0)
		portal_y += 5;
}


void Grid::incPortal_y(void){

	if(zoom ==1)
		return;

	portal_y += 5;
	
	int row_offset_down = rows/2 + rows/(2*zoom) + portal_y;

	if(row_offset_down > screen->h)
		portal_y -= 5;

}



void Grid::inputHandler(const SDL_Event &event){

	if(event.type == SDL_KEYDOWN){
		switch(event.key.keysym.sym){
			
			case SDLK_LEFT:		if(pvel_x>=0) pvel_x -= 1;
						break;

			case SDLK_RIGHT:	if(pvel_x<=0) pvel_x += 1;
						break;

			case SDLK_UP:		if(pvel_y>=0) pvel_y -= 1;
						break;

			case SDLK_DOWN:		if(pvel_y<=0) pvel_y += 1;
						break;

			default:		break;
		}
	
	}else if(event.type == SDL_KEYUP){
		switch(event.key.keysym.sym){
			
			case SDLK_LEFT:		if(pvel_x<=0) pvel_x += 1;
						break;

			case SDLK_RIGHT:	if(pvel_x>=0) pvel_x -= 1;
						break;

			case SDLK_UP:		if(pvel_y<=0) pvel_y += 1;
						break;

			case SDLK_DOWN:		if(pvel_y>=0) pvel_y -= 1;
						break;

			default:		break;

	
		}

	}
}


void Grid::update(void){

	if(zoom == 1)
		return;

	portal_x += pvel_x;
	portal_y += pvel_y;
	
	int row_offset_up = rows/2 - rows/(2*zoom) + portal_y;
	int row_offset_down = rows/2 + rows/(2*zoom) + portal_y;
	int col_offset_left = cols/2 - cols/(2*zoom) + portal_x;
	int col_offset_right = cols/2 + cols/(2*zoom) + portal_x;

	if((col_offset_left < 0) || ( (col_offset_right > screen->w) && pvel_x>0)) 
		portal_x -= pvel_x;

	if((row_offset_up < 0) || ( (row_offset_down > screen->h) && pvel_y>0))
		portal_y -= pvel_y;

}



void Grid::writeBMP(std::string file){

	int last_portal_x = portal_x;
	int last_portal_y = portal_y;
	int last_zoom = zoom;

	zoomReset();

	SDL_Surface *image = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel, \
						  screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, \
						  screen->format->Amask);

	draw(image);

	SDL_SaveBMP(image, file.c_str());

	zoom = last_zoom;
	portal_x = last_portal_x;
	portal_y = last_portal_y;

	SDL_FreeSurface(image);


}



void Grid::readBMP(std::string file){

	zoomReset();
	population = generation = 0;
	clearBoard();

	SDL_Surface *rawimage = SDL_LoadBMP(file.c_str());
	SDL_Surface *image = SDL_DisplayFormat(rawimage);
/*	blitSurface(0, 0, image, screen);
	SDL_Flip(screen);
	sleep(8);
	return;
*/
	int x=0, y=0;
	Uint32 *pixel;

	//fprintf(stderr, "image ByPP: %d\n", image->format->BytesPerPixel);

	for(y=0; y < image->h; ++y){
		for(x=0; x < image->w; ++x){
			pixel = (Uint32 *)image->pixels + y*image->pitch/image->format->BytesPerPixel + x;

			//fprintf(stderr, "At pixel[%d][%d]...\n",y, x);

			if(*pixel){
				board[y][x].state = 1;
				board[y][x].laststate = 1;
				//fprintf(stderr, "board[%d][%d] Alive\n", y, x);
				population++;
			}else{
			
				board[y][x].state = 0;
				board[y][x].laststate = 0;
			}
			board[y][x].age = 0;
		}
	}


}



extern "C"
void *Grid::thread_step(void *param){

	int neighbours, col_left, col_right, r, c, l_population=0;
	Cell *topleft, *top, *topright, *right, *bottomright, *bottom, *bottomleft, *left;

	ThreadParam *p = static_cast<ThreadParam *>(param);

	//fprintf(stderr, "Thread %d  Start:%d  End: %d of %d\n", p->tid, p->row_start, p->row_end, rows);

	for(r=p->row_start; r<=p->row_end; ++r){
		for(c=0; c<cols; ++c){

			if(c==0)
				col_left = cols-1;
			else
				col_left = c-1;

			if(c==(cols-1))
				col_right = 0;
			else
				col_right = c+1;


			neighbours = 0;

			if(r==p->row_start){
				topleft = &p->halo_n[col_left];
				top = &p->halo_n[c];
				topright = &p->halo_n[col_right];
				neighbours += topleft->state;
				neighbours += top->state;
				neighbours += topright->state;
			}else{
				topleft = &board[r-1][col_left];
				top = &board[r-1][c];
				topright = &board[r-1][col_right];
				neighbours += topleft->laststate;
				neighbours += top->laststate;
				neighbours += topright->laststate;
			}

			right = &board[r][col_right];

			if(r==p->row_end){
				bottomright = &p->halo_s[col_right];
				bottom = &p->halo_s[c];
				bottomleft = &p->halo_s[col_left];
			}else{
				bottomright = &board[r+1][col_right];
				bottom = &board[r+1][c];
				bottomleft = &board[r+1][col_left];
			}
			
			left = &board[r][col_left];

			if(c==cols-1)
				neighbours += right->laststate;
			else
				neighbours += right->state;

			neighbours += bottomright->state;
			neighbours += bottom->state;
			neighbours += bottomleft->state;

			if(c==0)
				neighbours += left->state;
			else
				neighbours += left->laststate;

			board[r][c].laststate = board[r][c].state;

			//fprintf(stderr, "[%d] Thread: %d  board[%d][%d]\n", generation, p->tid, r, c);


			if((neighbours < 2) || (neighbours > 3)){ // Death lonliness or overcrowding
				board[r][c].state = 0;
				board[r][c].age = 0;

				if(board[r][c].laststate == 1)		--l_population;
			
			}else if(neighbours == 3){	//Birth or continue

				board[r][c].state = 1;
				board[r][c].age++;
			
				if(board[r][c].laststate == 0)		++l_population;

			}else if(neighbours == 2){	//Stasis
				if(board[r][c].state == 1)		board[r][c].age++;
			}


		}
	}


	p->l_population = l_population;

	pthread_exit(static_cast<void *>(p));
}




void Grid::pstep(int threads){

	int i, c;
	int halo_n_row, halo_s_row;

	pthread_t *thread_id = new pthread_t[threads];
	ThreadParam *thread_param = new ThreadParam[threads];
	ThreadSet **ts = new ThreadSet *[threads];

	int row_slice = rows/threads;

	for(i=0; i<threads; ++i){
	
		thread_param[i].tid = i;
		thread_param[i].row_start = i*row_slice;
		thread_param[i].row_end = (i+1)*row_slice - 1;

		thread_param[i].halo_n = new Cell[cols];
		thread_param[i].halo_s = new Cell[cols];


		if(thread_param[i].row_start == 0)
			halo_n_row = rows-1;
		else
			halo_n_row = thread_param[i].row_start-1;

		if(thread_param[i].row_end == rows-1)
			halo_s_row = 0;
		else
			halo_s_row = thread_param[i].row_end+1;

		//fprintf(stderr, "[%d] Thread: %d halo_n: %d halo_s %d\n", generation, i, halo_n_row, halo_s_row); 

		memcpy(thread_param[i].halo_n, board[halo_n_row], cols*sizeof(Cell));
		memcpy(thread_param[i].halo_s, board[halo_s_row], cols*sizeof(Cell));
/*
		for(c=0; c<cols; ++c){
		
			thread_param[i].halo_n[c].state = board[halo_n_row][c].state;
			thread_param[i].halo_n[c].laststate = board[halo_n_row][c].laststate;
			//thread_param[i].halo_n[c].age = board[halo_n_row][c].age;
			thread_param[i].halo_s[c].state = board[halo_s_row][c].state;
			thread_param[i].halo_s[c].laststate = board[halo_s_row][c].laststate;
			//thread_param[i].halo_s[c].age = board[halo_s_row][c].age;
		
		}
*/
	}

	for(i=0; i<threads; ++i){
		ts[i] = new ThreadSet(this, &thread_param[i]);
		pthread_create(&thread_id[i], NULL, launchThread, static_cast<void *>(ts[i]) );
	}

	void *rv;

	for(i=0; i<threads; ++i){
	
		pthread_join(thread_id[i], &rv);

		population += (static_cast<ThreadParam *>(rv))->l_population;

		delete [] thread_param[i].halo_n;
		delete [] thread_param[i].halo_s;
		delete ts[i];
	
	}

	delete [] thread_param;
	delete [] thread_id;
	delete [] ts;

	generation++;
}



