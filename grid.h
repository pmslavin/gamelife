#ifndef GRID_H
#define GRID_H

#include "cell.h"
#include "sdlenv.h"

class Grid {

private:
	const int rows, cols;
	int population, generation, zoom, portal_x, portal_y, pvel_x, pvel_y;
       	double	initProbability;
	Cell **board;

	void initialise(void);
	void clearBoard(void);

public:
	Grid(const int r, const int c, double prob);
	~Grid(void);
	void step(void);
	void pstep(int threads);
	void reset(double prob);
	void report(void);
	void draw(SDL_Surface *dest);
	int getGen(void) { return generation; }
	int getPop(void) { return population; }
	void zoomIn(void) { ++zoom; }
	//void zoomOut(void) { if(zoom>1)  --zoom; if(zoom == 1) { portal_x = portal_y = 0;} }
	void zoomOut(void);
	void zoomReset(void) { zoom = 1; portal_x = portal_y = 0;  }
	int getZoom(void) { return zoom; }
	void decPortal_x(void);
	void incPortal_x(void);
	void decPortal_y(void);
	void incPortal_y(void);
	void inputHandler(const SDL_Event &event);
	void update(void);
	void writeBMP(std::string file);
	void readBMP(std::string file);
	void *thread_step(void *); 

};


#endif
