#ifndef THREADSET_H
#define THREADSET_H

#include "grid.h"

class ThreadSet{

public:
	Grid *thisGrid;
	void *gridArgs;
	ThreadSet(Grid *gr, void *args) : thisGrid(gr), gridArgs(args) {}

	void *launch(void) { return thisGrid->thread_step(gridArgs); }


};


#endif
