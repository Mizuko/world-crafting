#include "world.h"
//Voronoi Tile functions

void voronoiTiles(WORLD* w, char count) {
	char n = 0;
	POINT ps[count];
	char plateDirs[count];
	for (; n < count; n++) {
		ps[n].x = random()%W_WIDTH;
		ps[n].y = random()%W_HEIGHT;
		if (n == 0)
			plateDirs[n] = random()%2;
		else 
			plateDirs[n] = (-1)*(plateDirs[n-1]-1);
	}
	
	int a,b,c;
	tripleFor(a,b,c) {
		POINT* p = coordinateFromRelativeIndex(a,b,c);
		n = 0;
		int closest = -1;
		double c_dist = W_AREA + 1;
		for (; n < count; n++) {
			double distance = worldDistance(p, ps + n);
			if (c_dist > distance) {
				c_dist = distance;
				closest = n;
			}
			if (distance == 0) {
				//setHeight(w->tileRelative(a,b,c), (n+1));
			}
		}
		//printf("%d:", getHeight(w->tileRelative(a,b,c)));
		setPlateID(w->tileRelative(a,b,c), closest+1);
		setPlateDirection(w->tileRelative(a,b,c), plateDirs[closest]);
		/*printf("%x won (%d %d %d) %c\n", 
				closest, 
				a, b, c,
				plateDirs[closest] ? 'u' : 'd'
		);*/
		//printf("%d\n", getHeight(w->tileRelative(a,b,c)));
	}}}
}

void blurPlateLines(WORLD* w, int plateCount) {
	int* plateFreq = (int*)calloc(plateCount, sizeof(int));
	int n,a,b,c;
	tripleFor(a,b,c) {
		for (n = 0; n < plateCount; n++) {
			plateFreq[n] = 0;
		}
		int count = 0;
		POINT* p = coordinateFromRelativeIndex(a,b,c);
		int x = p->x;
		int y = p->y;
		if (p->x > 0) {
			if (p->y > 0) {
				plateFreq[getPlateID(w->tileCoordinate((x-1), (y-1))) - 1]++;
				count++;
			}
			plateFreq[getPlateID(w->tileCoordinate((x-1), y)) - 1]++;
			count++;
			if (p->y < W_HEIGHT-1) {
				plateFreq[getPlateID(w->tileCoordinate((x-1), (y+1))) - 1]++;
				count++;
			}
		}
		
		if (p->y > 0) {
			plateFreq[getPlateID(w->tileCoordinate(x, (y-1))) - 1]++;
			count++;
		}
		plateFreq[getPlateID(w->tileCoordinate(x, y)) - 1]++;
		count++;
		plateFreq[getPlateID(w->tileCoordinate(x, y)) - 1]++;
		count++;
		if (p->y < W_HEIGHT-1) {
			plateFreq[getPlateID(w->tileCoordinate(x, (y+1))) - 1]++;
			count++;
		}
		if (p->x < W_WIDTH-1) {
			if (p->y > 0) {
				plateFreq[getPlateID(w->tileCoordinate((x+1), (y-1))) - 1]++;
				count++;
			}
			plateFreq[getPlateID(w->tileCoordinate((x+1), y)) - 1]++;
			count++;
			if (p->y < W_HEIGHT-1) {
				plateFreq[getPlateID(w->tileCoordinate((x+1), (y+1))) - 1]++;
				count++;
			}
		}
		if (count != 0) {
			int roll = random()%count;
			for (n = 0; n < plateCount; n++) {
				roll -= plateFreq[n];
				if (roll < 0) {
					setPlateID(w->tileRelative(a,b,c), n+1);
					n = plateCount;
				}
			}
		}
	}}}
	free(plateFreq);
}