TARGET	:= gol
CC	:= g++ -g -pthread
OBJECTS	:= sdlenv.o cell.o threadparam.o grid.o statusbar.o panel.o help.o about.o gol.o
WARN	:= -W -Wall
LIBS	:= -lSDL -lSDL_ttf -lpthread
SDLFLAGS:= `sdl-config --cflags` -L.

all:	gol

gol:	${OBJECTS}	
	${CC} ${WARN} ${SDLFLAGS} ${OBJECTS}  -o gol ${LIBS}

sdlenv.o: sdlenv.cpp sdlenv.h
	${CC} ${WARN} ${SDLFLAGS} -c sdlenv.cpp ${LIBS}

cell.o: cell.cpp cell.h
	${CC} ${WARN} ${SDLFLAGS} -c cell.cpp ${LIBS}

grid.o: grid.cpp grid.h
	${CC} ${WARN} ${SDLFLAGS} -c grid.cpp ${LIBS}

gol.o: gol.cpp 
	${CC} ${WARN} ${SDLFLAGS} -c gol.cpp ${LIBS}

statusbar.o: statusbar.cpp statusbar.h
	${CC} ${WARN} ${SDLFLAGS} -c statusbar.cpp ${LIBS}

panel.o: panel.cpp panel.h
	${CC} ${WARN} ${SDLFLAGS} -c panel.cpp ${LIBS}

help.o: help.cpp help.h
	${CC} ${WARN} ${SDLFLAGS} -c help.cpp ${LIBS}

about.o: about.cpp about.h
	${CC} ${WARN} ${SDLFLAGS} -c about.cpp ${LIBS}

threadparam.o: threadparam.cpp threadparam.h
	${CC} ${WARN} ${SDLFLAGS} -c threadparam.cpp ${LIBS}

clean:
	-rm -f *.o gol
