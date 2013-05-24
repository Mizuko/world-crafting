
#ifndef __WORLD_H
#define __WORLD_H

#include <stdlib.h>
#include <math.h>

typedef struct _point {
	int x;
	int y;
} POINT;

POINT* Point(int x, int y);

//planned size of a view
#define V_WIDTH 80
#define V_HEIGHT 25
#define V_AREA (V_WIDTH*V_HEIGHT)

//Zoom for what size square should be simplified to one tile on the world map
//Should be an even divisor of V_WIDTH*V_HEIGHT
#define ZOOM_FACTOR 5
#define ZOOM_FACTOR_SQRD (ZOOM_FACTOR*ZOOM_FACTOR)

#define W_WIDTH (ZOOM_FACTOR*V_WIDTH)
#define W_HEIGHT (ZOOM_FACTOR*V_HEIGHT)
#define W_AREA (V_AREA*ZOOM_FACTOR_SQRD)
#define EQUATOR W_HEIGHT/2

#define A_MAX ZOOM_FACTOR_SQRD
#define B_MAX (V_AREA/ZOOM_FACTOR_SQRD)
#define C_MAX ZOOM_FACTOR_SQRD

#define tripleFor(a,b,c) for(a=0;a<A_MAX;a++){for(b=0;b<B_MAX;b++){for(c=0;c<C_MAX;c++)
#define doubleFor(x,y) for(x=0;x<W_WIDTH;x++){for(y=0;y<W_HEIGHT;y++)

#define SEA_LEVEL 30
#define HEIGHT_RADIUS 4
#define PLATE_STEPS 6
#define ISLAND_MOD 1

#define FLAT_MAX 2.25
#define HILL_MIN 0.66
#define HILL_MAX 4
#define HILL_DAMPENER 3.05
#define MOUNTAIN_MIN_BOOSTER 2.1
#define PEAK_HEIGHT 4.66

#define SAT_RATIO (1.0/(double)HEIGHT_RADIUS)
#define SAT_RADIUS 5
#define SAT_STEP 5
#define SAT_MIN 22

#define HADLEY_RADIUS (1.0/3.0)
#define HADLEY_MAX 222
#define HADLEY_MIN SAT_MIN

#define TEMP_MIN 40
#define TEMP_STEP 2.5
#define TEMP_MAX (TEMP_MIN + ((W_HEIGHT/2)*TEMP_STEP))

//Coasts within POLE_WIDTH of the Height edge don't trigger modification
#define POLE_WIDTH 3
#define COAST_TEMP_MOD 20
#define COAST_TEMP_POWER 222
#define COAST_TEMP_HEIGHT_RATIO 0.75

#define RAIN_TOTAL 6.5*SAT_RADIUS
#define RAIN_POWER_MAX 222
#define RAIN_POWER_RATIO (4.0/3.0)
#define RAIN_HEIGHT_RATIO 0.75

#define viewRelative(a) views[a]
#define viewCoordinate(x, y) viewRelative(_a(x,y))

#define blockRelative(a, b) viewRelative(a).blocks[b]
#define blockCoordinate(x, y) blockRelative(_a(x,y), _b(x,y))

#define tileRelative(a, b, c) blockRelative(a, b).tiles[c]
#define tileCoordinate(x, y) tileRelative(_a(x,y), _b(x,y), _c(x,y))

//Helpers for tileCoordinate
#define _a(x, y) (x/V_WIDTH + ZOOM_FACTOR*(y/V_HEIGHT))
#define _b(x, y) ((x%V_WIDTH)/ZOOM_FACTOR) + (V_WIDTH/ZOOM_FACTOR)*((y%V_HEIGHT)/ZOOM_FACTOR)
#define _c(x, y) ((x%V_WIDTH)%ZOOM_FACTOR) + ZOOM_FACTOR*((y%V_HEIGHT)%ZOOM_FACTOR)

//Helpers for tile mask operations
#define _setMask(t, n, mask, offset) t &= (~((TILE)mask << offset)); t |= ((TILE)(n & mask) << offset);
#define _getMask(t, mask, offset) ( (t >> offset) & (TILE)mask )

//0000 0000 nnnn nnnn
#define HEIGHT_MASK 0xFF
#define HEIGHT_OFFSET 0
#define setHeight(t, n) _setMask(t, n, HEIGHT_MASK, HEIGHT_OFFSET)
#define getHeight(t) _getMask(t, HEIGHT_MASK, HEIGHT_OFFSET)

//0nnn nnnn 0000 0000
#define PLATE_ID_MASK 0x7F
#define PLATE_ID_OFFSET 8
#define setPlateID(t, n) _setMask(t, n, PLATE_ID_MASK, PLATE_ID_OFFSET)
#define getPlateID(t) _getMask(t, PLATE_ID_MASK, PLATE_ID_OFFSET)

//n000 0000 0000 0000
#define PLATE_DIR_MASK 1
#define PLATE_DIR_OFFSET 15
#define setPlateDirection(t, n) _setMask(t, n, PLATE_DIR_MASK, PLATE_DIR_OFFSET) 
#define getPlateDirection(t) _getMask(t, PLATE_DIR_MASK, PLATE_DIR_OFFSET)

//0000 nnnn 0000 0000 0000 0000 0000 0000
#define THEME_MASK 0xF
#define THEME_OFFSET 24
#define setTheme(t, n) _setMask(t, n, THEME_MASK, THEME_OFFSET)
#define getTheme(t) _getMask(t, THEME_MASK, THEME_OFFSET)

//nnnn 0000 0000 0000 0000 0000 0000 0000
#define COAST_MASK 0xF
#define COAST_OFFSET 28
#define setCoastFlag(t, n) _setMask(t, n, COAST_MASK, COAST_OFFSET)
#define getCoastFlag(t) _getMask(t, COAST_MASK, COAST_OFFSET)

//nnnn nnnn 0000 0000
#define SAT_MASK 0xFF
#define SAT_OFFSET 8
#define setSaturation(t, n) _setMask(t, n, SAT_MASK, SAT_OFFSET)
#define getSaturation(t) _getMask(t, SAT_MASK, SAT_OFFSET)

//nnnn nnnn 0000 0000 0000 0000
#define TEMP_MASK 0xFF
#define TEMP_OFFSET 12
#define setTemp(t, n) _setMask(t, n, TEMP_MASK, TEMP_OFFSET)
#define getTemp(t) _getMask(t, TEMP_MASK, TEMP_OFFSET)

//Themes
#define WATER 0
#define FLAT 1
#define HILLS 2
#define MOUNTAIN 3
#define PEAK 4
//Arid plains/hills
#define DESERT 5
//Tropical terrains?
//Marsh?
#define TUNDRA 6
#define ICE 7
#define FOREST 10

typedef unsigned long int TILE;

typedef struct _block {
	TILE tiles[C_MAX];
} BLOCK;

typedef struct _view {
	BLOCK blocks[B_MAX];
} VIEW;

typedef struct _world {
	VIEW views[A_MAX];
} WORLD; 

TILE blockSummary(BLOCK b);

POINT* coordinateFromRelativeIndex(int view, int block, int tile);

void voronoiTiles(WORLD* w, char count);

void blurPlateLines(WORLD* w, int plateCount);

double worldDistance(POINT* p1, POINT* p2);

POINT* moveFrom(POINT* p, int dx, int dy);

WORLD* newWorld(int seed);

WORLD* newWorldRandom();

void saveWorld(WORLD* w, const char* name);

WORLD* loadWorld(const char* name);
#endif //__WORLD_H