#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "point.h"

typedef struct gameloopStateStruct {
	double lastLoopTime;
	double timeAccumulatedMs;

	/* 
	* 0 - not dragging
	* 1 - drag in progress
	* 2 - stopped dragging - use calculation
	*/
	int startedDragging;

	double mousePrevX = 0;
	double mousePrevY = 0;
	Point moveBy;
	double dx = 0;
	double dy = 0;

} GameloopState;

extern GameloopState gameloopState;


#endif
