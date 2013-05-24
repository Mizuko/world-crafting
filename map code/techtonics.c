#include "plates.c"

//plate techtonics simulations

TILE collisionResult(TILE one, TILE two, int driftEnd) {
	TILE r = 0;
	int oneDir = getPlateDirection(one);
	int twoDir = getPlateDirection(two);
	int oneID = getPlateID(one);
	int twoID = getPlateID(two);
	int oneHeight = getHeight(one);
	int twoHeight = getHeight(two);
	if (oneDir == twoDir & oneDir == 1) {
		//over v. over
		//Increase height
		int height = (oneHeight > twoHeight ? oneHeight : twoHeight);
		int subMod = ((oneHeight > twoHeight ? twoHeight : oneHeight) - SEA_LEVEL);
		if (subMod < 0)
			subMod = 0;
		height += subMod;
		height += 1 + random()%2;
		setHeight(r, height);
		
		//end Drift
		setPlateID(r, driftEnd);
		setPlateDirection(r, 1);
	}
	
	if (oneDir == twoDir & oneDir == 0) {
		//under v. under
		//increase height if already SEA_LEVEL - ISLAND_MOD
		int height = (oneHeight > twoHeight ? oneHeight : twoHeight);
		if (height > (SEA_LEVEL - ISLAND_MOD))
			height += ISLAND_MOD + random()%3;
		setHeight(r, height);
		
		//end Drift
		setPlateID(r, driftEnd + 1);
		if (height > SEA_LEVEL) {
			setPlateDirection(r, 1);
		} else {
			setPlateDirection(r, 0);
		}
	}
	
	if (oneDir != twoDir & oneDir == 0) {
		//one: under
		//two: over
		//give a small increase to the over height
		setHeight(r, (twoHeight + 1 + (random()%2)) );
		
		//tile should follow the over plate
		setPlateDirection(r, 1);
		setPlateID(r, twoID);
	}
	
	if (oneDir != twoDir & oneDir == 1) {
		//one: over
		//two: under
		
		//give a small increase to the over height
		setHeight(r, (oneHeight + 1 + (random()%2)) );
		
		//tile should follow the over plate
		setPlateDirection(r, 1);
		setPlateID(r, oneID);
	}
	
	return r;
}

void movePlates(WORLD* w, int plateCount) {
	int n,a,b,c,x,y;
	WORLD* buffer = (WORLD*)malloc(sizeof(WORLD));
	tripleFor(a,b,c) {
		TILE t = w->tileRelative(a,b,c);
		if (getPlateDirection(t) == 0) {
			setHeight(w->tileRelative(a,b,c), getHeight(t) - HEIGHT_RADIUS);
		} else if (PLATE_STEPS == 1) {
			setHeight(w->tileRelative(a,b,c), getHeight(t) + HEIGHT_RADIUS);
		}
		buffer->tileRelative(a,b,c) = 0;
	}}}
	//init speeds
	int* speeds = (int*)calloc(plateCount*2, sizeof(int));
	for (n = 0; n < plateCount*2; n++) {
		speeds[n] = -12+random()%24;
	}
	
	//moves
	for (n = 0; n < PLATE_STEPS; n++) {
		//printf("%d\n", n);
		doubleFor(x,y) {
			TILE t = w->tileCoordinate(x,y);
			
			int plateID = getPlateID(t) - 1;
			if (plateID > plateCount) {
				continue;
			}
			
			int* dx = speeds + (plateID * 2);
			int* dy = speeds + 1 + (plateID*2);
			POINT* p = moveFrom(Point(x,y), *dx, *dy);
			int nx = p->x;
			int ny = p->y;
			//Passed the poles
			/*if (ny < (-1)*POLE_WIDTH)
				ny = W_HEIGHT - (ny + POLE_WIDTH);
			else if (ny > W_HEIGHT + POLE_WIDTH)
				ny = (ny%W_HEIGHT)-POLE_WIDTH;*/
			
			//Too over the pole
			if (ny < 0 | ny > W_HEIGHT) 
				continue;
			
			TILE nt = buffer->tileCoordinate(nx,ny);
			if (getPlateID(nt) != 0) {
				//There's already a plate there
				buffer->tileCoordinate(nx,ny) = collisionResult(nt, t, plateCount + 2);
			} else {
				//no collision, no contest
				buffer->tileCoordinate(nx,ny) = t;
			}
		}}
		
		int i;
		for (i = 0; i < plateCount*2; i++) {
			speeds[i] = ((double)(speeds[i])*0.84);
		}	
		
		doubleFor(x,y) {
			TILE t = buffer->tileCoordinate(x,y);
			if (getHeight(t) == 0) {
				setHeight(w->tileCoordinate(x,y), (SEA_LEVEL - (3*HEIGHT_RADIUS)));
				setPlateID(w->tileCoordinate(x,y), plateCount + 4);
				setPlateDirection(w->tileCoordinate(x,y), 0);
			} else {
				//printf("nb\n");
				w->tileCoordinate(x,y) = t;
			}		
		}}
	}
	free(buffer);
	free(speeds);
}

int transfer(WORLD* w, int* height, int x, int y) {
	if ((*height) <= (SEA_LEVEL))
		return 0;
		
	int oH = getHeight(w->tileCoordinate(x,y));
	if (oH > (SEA_LEVEL))
		return 0;
		
	(*height)--;
	oH++;
	setHeight(w->tileCoordinate(x,y), oH);
	return 1;
}
 
int fillHoles(WORLD* w) {
	int total = 0;
	int x,y;
	doubleFor(x,y) {
		int height = getHeight(w->tileCoordinate(x,y));
		if (x > 0) {
			total += transfer(w, &height, x-1, y);
		}
		
		if (y > 0) {	
			total += transfer(w, &height, x, y-1);
		}
		
		if (y < W_HEIGHT-1) {
			total += transfer(w, &height, x, y+1);
		}
		
		if (x < W_WIDTH-1) {
			total += transfer(w, &height, x+1, y);
		}
		
		if (x > 1) {
			total += transfer(w, &height, x-2, y);
		}
		
		if (y > 1) {	
			total += transfer(w, &height, x, y-2);
		}
		
		if (y < W_HEIGHT-2) {
			total += transfer(w, &height, x, y+2);
		}
		
		if (x < W_WIDTH-2) {
			total += transfer(w, &height, x+2, y);
		}
		setHeight(w->tileCoordinate(x,y), height);
	}}
	return total;
}

void seedMods(int* mods, int l, int h, int base, double mod) {
	if (l == h) 
		return;
	int roll = 1+random()%2;
	roll *= mod;
	roll *= ((2*((random()%3)>1))-1);
	base += roll;
	
	int mid = (l+h)/2;
	mods[mid] = base;
	if (mid != h)
		seedMods(mods, l, mid, base, mod/2.0);
	if (mid != l)
		seedMods(mods, mid, h, base, mod/2.0);
}

void modLand(WORLD* w, int mod, int x, int y, int facing, int direction) {
	//facing 1 = land is left/up
	//facing 0 = land is right/down
	//direction 1; vertical
	//direction 0; horz
	
	int sinkFlag = 0;
	if (mod < 0) {
		if (facing == 1) {
			sink(w, x, y);
			sinkFlag = 1;
		}
	}
	
	if (mod > 0) {
		if (facing == 0) {
			sink(w, x, y);
			sinkFlag = 1;
		}
	}
	while (mod != 0) {
		if (direction == 1) {
			int nx = x + mod;
			if ((nx >= 0) & (nx < W_WIDTH)) {
				if (sinkFlag == 1) {
					sink(w, nx, y);
				} else {
					rise(w, nx, y);
				}			
			}
		}
		
		if (direction == 0) {
			int ny = y + mod;
			if ((ny >= 0) & (ny < W_HEIGHT)) {
				if (sinkFlag == 1) {
					sink(w, x, ny);
				} else {
					rise(w, x, ny);
				}			
			}
		}
		
		if (mod > 0) {
			mod--;
		} else {
			mod++;
		}
	}
}

void moveLine(WORLD* w, int d, int min, int max, int facing, int direction) {
	//facing 1 = land is left/up
	//facing 0 = land is right/down
	//direction 1; vertical
	//direction 0; horz
	
	int range = max - min;
	int* mods = (int*)calloc(range, sizeof(int));
	int n;
	for (n = 0; n < range; n++) {
		mods[n] = 0;
	}
	seedMods(mods, 0, range-1, 0, 1.0);
	
	for (n = 0; n < range; n++) {
		if (direction == 1) {
			modLand(w, mods[n], d, min+n, facing, direction);
		} else {
			modLand(w, mods[n], min+n, d, facing, direction);
		}
	}
}

int blurCoasts(WORLD* w, int minLen) {
	int totalLength = 0;
	int x,y,min,max,length,lineFlag = 0;
	doubleFor(x,y) {
		if (y*x == 0)
			continue;
		TILE t = w->tileCoordinate(x,y);
		if (lineFlag == 0) { //No line ID
			if (x == W_WIDTH - 1)
				continue;
			TILE u = w->tileCoordinate(x,(y-1));
			if ((getHeight(t) > SEA_LEVEL) & (getHeight(u) > SEA_LEVEL)) {
				TILE l = w->tileCoordinate((x-1), y);
				TILE r = w->tileCoordinate((x+1), y);
				int lSea = (getHeight(l) <= SEA_LEVEL);
				int rSea = (getHeight(r) <= SEA_LEVEL);
				if (lSea != rSea) {
					length = 2;
					if (lSea)
						lineFlag = -1;
					else
						lineFlag = 1;
					min = y;
				}
			}
		} else if (lineFlag == 1) { //Left land
			if (getHeight(t) > SEA_LEVEL) {
				TILE l = w->tileCoordinate((x-1), y);
				TILE r = w->tileCoordinate((x+1), y);
				int lSea = (getHeight(l) <= SEA_LEVEL);
				int rSea = (getHeight(r) <= SEA_LEVEL);
				if ((lSea == 0) & rSea) {
					length++;
				} else {
					max = y;
					lineFlag = 2;
				}
			} else {
				max = y-1;
				lineFlag = 2;
			}
		} else if (lineFlag == -1) { //Right land
			if (getHeight(t) > SEA_LEVEL) {
				TILE l = w->tileCoordinate((x-1), y);
				TILE r = w->tileCoordinate((x+1), y);
				int lSea = (getHeight(l) <= SEA_LEVEL);
				int rSea = (getHeight(r) <= SEA_LEVEL);
				if (lSea & (rSea == 0)) {
					length++;
				} else {
					max = y;
					lineFlag = -2;
				}
			} else {
				max = y-1;
				lineFlag = -2;
			}
		}
		
		if (lineFlag == 2) { //left finish
			lineFlag = 0;
			if (length <= minLen)
				continue;
			
			if (max == 0)
				continue;
			
			moveLine(w, x, min, max, 1, 1);
			totalLength += length;
		}
		
		if (lineFlag == -2) { //right finish
			lineFlag = 0;
			if (length <= minLen)
				continue;
			
			if (max == 0)
				continue;
				
			moveLine(w, x, min, max, 0, 1);
			totalLength += length;
		}
	}}
	
	
	lineFlag = 0;
	for(y=1;y<W_HEIGHT;y++) {
	for(x=1;x<W_WIDTH;x++) {
		TILE t = w->tileCoordinate(x,y);
		if (lineFlag == 0) { //No line ID
			if (y == W_HEIGHT - 1)
				continue;
			TILE l = w->tileCoordinate((x-1),y);
			if ((getHeight(t) > SEA_LEVEL) & (getHeight(l) > SEA_LEVEL)) {
				TILE up = w->tileCoordinate(x, (y-1));
				TILE dn = w->tileCoordinate(x, (y+1));
				int upSea = (getHeight(up) <= SEA_LEVEL);
				int dnSea = (getHeight(dn) <= SEA_LEVEL);
				if (upSea != dnSea) {
					length = 2;
					if (dnSea)
						lineFlag = -1;
					else
						lineFlag = 1;
					min = x;
				}
			}
		} else if (lineFlag == 1) { //up land
			if (getHeight(t) > SEA_LEVEL) {
				TILE u = w->tileCoordinate(x, (y-1));
				TILE d = w->tileCoordinate(x, (y+1));
				int uSea = (getHeight(u) <= SEA_LEVEL);
				int dSea = (getHeight(d) <= SEA_LEVEL);
				if ((dSea == 0) & uSea) {
					length++;
				} else {
					max = x;
					lineFlag = 2;
				}
			} else {
				max = x-1;
				lineFlag = 2;
			}
		} else if (lineFlag == -1) { //down land
			if (getHeight(t) > SEA_LEVEL) {
				TILE u = w->tileCoordinate(x, (y-1));
				TILE d = w->tileCoordinate(x, (y+1));
				int uSea = (getHeight(u) <= SEA_LEVEL);
				int dSea = (getHeight(d) <= SEA_LEVEL);
				if (dSea & (uSea == 0)) {
					length++;
				} else {
					max = x;
					lineFlag = -2;
				}
			} else {
				max = x-1;
				lineFlag = -2;
			}
		}
		
		if (lineFlag == 2) { //up finish
			lineFlag = 0;
			if (length <= minLen)
				continue;
			
			if (max == 0)
				continue;

			//printf("Up line (%d %d) %d (%d %d)\n", min, y, length, max, y);
			moveLine(w, y, min, max, 0, 0);
			totalLength += length;
		}
		
		if (lineFlag == -2) { //down finish
			lineFlag = 0;
			if (length <= minLen)
				continue;
			
			if (max == 0)
				continue;
				
			//printf("Down line (%d %d) %d (%d %d)\n", min, y, length, max, y);
			moveLine(w, y, min, max, 1, 0);
			totalLength += length;
		}
	}}
	
	return totalLength;
}


void plateTechtonics(WORLD* w) {
	int plateCount = ZOOM_FACTOR_SQRD - ZOOM_FACTOR;
	voronoiTiles(w, plateCount);
	
	//makes the plates a bit more interesting than polygons
	blurPlateLines(w, plateCount);
	blurPlateLines(w, plateCount);
	
	//Techtonic those plates!
	movePlates(w, plateCount);
	
	//The land looks like swiss cheese!
	//Try filling the seas with height from neighboring land
	//until the previous operation filled in fewer holes than this one
	int n = fillHoles(w), t = 0;
	while(n > (t = fillHoles(w))) n = t;
// 	while (n){
// 		printf("%d %d\n", n, t=fillHoles(w));	
// 		n--;
// 	}
	
	//This function was more needed before fillHoles was changed
	//But it takes coasts that are a straight line for more than n tiles
	//and shuffles them a bit. Increase n until there are no coasts modified
	n = 6;
	while (blurCoasts(w,n++) != 0);
	
	//but this can cut into lands and make mountains sad. Let's fill holes again
	n = fillHoles(w), t = 0;
	while(n > (t = fillHoles(w))) n = t;
}
