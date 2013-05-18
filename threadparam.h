#ifndef THREAD_PARAM_H
#define THREAD_PARAM_H

#include "cell.h"

class ThreadParam{

public:
	int tid, row_start, row_end, l_population;
	Cell *halo_n, *halo_s;
	ThreadParam(int t, int rs, int re);
	ThreadParam(void) : l_population(0) {}


};



#endif
