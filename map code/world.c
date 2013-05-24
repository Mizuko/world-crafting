#include <stdlib.h>
#include <stdio.h>
#include "../util/simplexnoise.h"
#include "world.h"
#include "worldutil.c"
#include "techtonics.c"
#include "tileinit.c"
#include "waterdist.c"
#include "coastal.c"
#include "temp.c"

//World creation and debug

void addLandNoise(WORLD* w) {
	voronoiTiles(w, ZOOM_FACTOR_SQRD);
	blurPlateLines(w, ZOOM_FACTOR_SQRD);

	int a,b,c;	
	tripleFor(a,b,c) {
		POINT* p = coordinateFromRelativeIndex(a,b,c);
		int height = SEA_LEVEL;

		double r = scaled_octave_noise_2d(5.0f, .5f, ZOOM_FACTOR, 0, 1, p->x, p->y);
		int dir = (((random()%2)*2)-1);
		//(((random()%2)*2)-1);
		//((getPlateDirection(w->tileRelative(a, b, c))*2)-1);
		height += dir*(r*HEIGHT_RADIUS);
		double rx = random()%2;
		double ry = random()%2;
		if (rx == 0 || p->x == W_WIDTH-1) {
			rx = scaled_octave_noise_2d(5.0f, .5f, ZOOM_FACTOR, 0, 1, p->x - 1, p->y);
		} else {
			rx = scaled_octave_noise_2d(5.0f, .5f, ZOOM_FACTOR, 0, 1, p->x + 1, p->y);			
		}

		if (ry == 0 || p->y == W_HEIGHT-1) {
			ry = scaled_octave_noise_2d(5.0f, .5f, ZOOM_FACTOR, 0, 1, p->x, p->y - 1);
		} else {
			ry = scaled_octave_noise_2d(5.0f, .5f, ZOOM_FACTOR, 0, 1, p->x, p->y + 1);		
		}
		
		height += dir*((rx/3.0)*HEIGHT_RADIUS) + dir*((rx/3.0)*HEIGHT_RADIUS);
		setHeight(w->tileRelative(a, b, c), height);
	}}}
}

void dbg_coordPrint() {
	int a,b,c;
	tripleFor(a,b,c) {
		printf("(%d, %d, %d) -> ", a, b, c);
		POINT* p = coordinateFromRelativeIndex(a,b,c);
		printf("(%d ,%d)", p->x, p->y);
		printf("%c\n", ((_a(p->x, p->y) != a) || (_b(p->x, p->y) != b) || (_c(p->x, p->y) != c)) ? '!' : ' ');
	}}}
}

void dbg_conversionTest(WORLD* w) {
	int a,b,c;
	tripleFor(a,b,c) {
		w->tileRelative(a,b,c) = a+b+c;
	}}}
	int i;
	for (i = 0; i < 4; i++) {
		a = random()%A_MAX;
		b = random()%B_MAX;
		c = random()%C_MAX;
		
		POINT* p = coordinateFromRelativeIndex(a, 0, 0);
		printf("%d:(%d %d) (%d %d) ->%lu\n\n", a, p->x, p->y, b, c, w->tileCoordinate((p->x + b), (p->y + c)));
		
	}
}

void dbg_printMinMax(WORLD* w) {
	int a,b,c,min,max;
	max = 0;
	min = 0xFFF;
	tripleFor(a,b,c){
		int height = getHeight(w->tileRelative(a,b,c));
		if (height < min)
			min = height;
		if (height > max)
			max = height;
	}}}
	
	printf("%d - %d\n", min, max);
}

WORLD* newWorld(int seed) {
	WORLD* w = (WORLD*)malloc(sizeof(WORLD));

	srandom(seed);
	
	addLandNoise(w);
	//dbg_printMinMax(w);
	
	plateTechtonics(w);
	
	themeInit(w);
	
	tempInit(w);
	coastalMods(w);
	
	tempBlur(w);
	tempBlur(w);
	
	//dbg_printMinMax(w);
	waterDistribution(w);
	return w;
}

WORLD* newWorldRandom() {
	return newWorld(random());
}
