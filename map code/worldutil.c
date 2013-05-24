#include "world.h"

//util functions

POINT* Point(int x, int y) {
	POINT* p = (POINT*)malloc(sizeof(POINT));
	p->x = x;
	p->y = y;
	return p;
}

TILE blockSummary(BLOCK b) {
	int n = 0, max = 0, skip = -1;
	int aggHeight = 0, aggPlate = 0, aggSat = 0, aggTemp = 0;
	int* themeFreq = (int*)calloc(0xF,sizeof(int));
	for (n = 0; n < 0xF; n++) {
		themeFreq[n] = 0;
	}

	for (n = 0; n < ZOOM_FACTOR_SQRD; n++) {
		TILE t = b.tiles[n];
		aggHeight += (getHeight(t) - SEA_LEVEL);
		aggSat += getSaturation(t);
		aggTemp += getTemp(t);
		themeFreq[getTheme(t)]++;
		//printf("h:%d p:%d\n", getHeight(t), getPlateID(t));
		
		if (getHeight(t) <= SEA_LEVEL)
			skip++;
	}

	//printf("(");
	for (n = 0; n < 0xF; n++) {
		//printf("%d:%d ", n, plateFreq[n]);
		if (themeFreq[n] > max) {
			max = themeFreq[n];
			aggPlate = n;
		}
	}
	
	TILE t = 0;
	setHeight(t, (aggHeight/ZOOM_FACTOR_SQRD) + SEA_LEVEL);
	setSaturation(t, (aggSat/ZOOM_FACTOR_SQRD));
	setTemp(t, (aggTemp/(ZOOM_FACTOR_SQRD-skip)));
	setTheme(t, aggPlate);
	free(themeFreq);
	return t;
}

POINT* coordinateFromRelativeIndex(int view, int block, int tile) {
	POINT* p = (POINT*)malloc(sizeof(POINT));
	p->x = (tile%ZOOM_FACTOR) + ZOOM_FACTOR*((block)%(V_WIDTH/ZOOM_FACTOR)) + (V_WIDTH *(view%ZOOM_FACTOR));
	p->y = (tile/ZOOM_FACTOR) + ZOOM_FACTOR*((block)/(V_WIDTH/ZOOM_FACTOR)) + (V_HEIGHT*(view/ZOOM_FACTOR));
	return p;
}

void sink(WORLD* w, int x, int y) {
	TILE t = w->tileCoordinate(x, y);
	int h = getHeight(t);
	if (h > SEA_LEVEL) {
		h = SEA_LEVEL - (h - SEA_LEVEL);
	}
	/*while (h > SEA_LEVEL) {
		h -= HEIGHT_RADIUS;
	}*/
	setHeight(t, h);
	w->tileCoordinate(x, y) = t;
}

void rise(WORLD* w, int x, int y) {
	TILE t = w->tileCoordinate(x, y);
	int h = getHeight(t);
	
	/*if (h < SEA_LEVEL) {
		h = SEA_LEVEL + (SEA_LEVEL - h);
	}*/
	while (h <= SEA_LEVEL) {
		h += HEIGHT_RADIUS;
	}
	setHeight(t, h);
	w->tileCoordinate(x, y) = t;
}

//Current Implementations;
//	X-wrap
//	Y-artic trunctation
double worldDistance(POINT* p1, POINT* p2) {
	int dy = p1->y - p2->y;
	int dx1 = p1->x - p2->x;
	int dx2 = W_WIDTH - dx1;
	dx1 *= dx1;
	dx2 *= dx2;
	return sqrt(dy*dy + (dx1 < dx2 ? dx1 : dx2));
}

TILE* adjacentTiles(WORLD* w, int x, int y) {
	TILE* adj = (TILE*)calloc(sizeof(TILE), 4);
	if (y > 0) {
		adj[3] = w->tileCoordinate(x, (y-1));
	} else
		adj[3] = 0;

	if (y < W_HEIGHT) {
		adj[2] = w->tileCoordinate(x, (y+1));
	} else {
		adj[2] = 0;
	}
	
	int dx = x - 1;
	if (dx == -1)
		dx = W_WIDTH - 1;
	adj[1] = w->tileCoordinate(dx, y);
	
	dx = (x + 1)%W_WIDTH;
	adj[0] = w->tileCoordinate(dx, y);
	
	return adj;
}

POINT* moveFrom(POINT* p, int dx, int dy) {
	return Point((W_WIDTH + p->x + dx)%W_WIDTH, p->y + dy);
}

void saveWorld(WORLD* w, const char* name) {
	FILE* f = fopen(name, "wb");
	fwrite(w, sizeof(WORLD), 1, f);
	fclose(f);
	
}

WORLD* loadWorld(const char* name) {
	WORLD* w = (WORLD*)malloc(sizeof(WORLD));
	FILE* f = fopen(name, "rb");
	if (f == NULL)
		return NULL;
	fread(w, sizeof(WORLD), 1, f);
	fclose(f);
	return w;
}
