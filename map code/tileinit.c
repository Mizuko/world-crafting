#include <math.h>
//initial theme creation

void subTile(WORLD* w, int dx, int dy) {
	int x,y,t1,t2,t3,t4,h1,h2,h3,h4;
	double p1,p2,p3,p4;
	for (x = dx; x < W_WIDTH-1; x += 2) {
		for (y = dy; y < W_HEIGHT-1; y += 2) {
		t1 = w->tileCoordinate(x,y);
		t2 = w->tileCoordinate((x+1), y);
		t3 = w->tileCoordinate((x+1), (y+1));
		t4 = w->tileCoordinate(x, (y+1));
		
		h1 = getHeight(t1);
		h2 = getHeight(t2);
		h3 = getHeight(t3);
		h4 = getHeight(t4);
		
		p1 = getTheme(t1);
		p2 = getTheme(t2);
		p3 = getTheme(t3);
		p4 = getTheme(t4);
		
		int water = (h1 <= SEA_LEVEL);
 		water += (h2 <= SEA_LEVEL);
 		water += (h3 <= SEA_LEVEL);
 		water += (h4 <= SEA_LEVEL);
		//Water
		//if the chunk is half water
		if (water > 1) {
			//Leave it alone
		} else {
		
		//Flat land
		//if the chunk isn't water and doesn't have any tile with height > SEA_LEVEL + (FLAT_MAX*HEIGHT_RADIUS)
		int flat = (h1 < (SEA_LEVEL + (FLAT_MAX * HEIGHT_RADIUS)));
		flat &= (h2 < (SEA_LEVEL + (FLAT_MAX * HEIGHT_RADIUS)));
		flat &= (h3 < (SEA_LEVEL + (FLAT_MAX * HEIGHT_RADIUS)));
		flat &= (h4 < (SEA_LEVEL + (FLAT_MAX * HEIGHT_RADIUS)));
		if (flat) {
			//sink each tile, than raise it?
			//...or i guess nothing?
			//if the height is > SEA_LEVEL + HILL_MIN*HEIGHT_RADIUS, use transform 2
			//otherwise, use transform 3
			#define heightTest(h, x) ((h > (SEA_LEVEL + (HILL_MIN*HEIGHT_RADIUS))) ? quadTransform02(x) : quadTransform03(x))
			
			//0->0 1->1 2->2 3->2
			#define quadTransform02(x) ((-0.25)*(x*x) + (1.45)*(x) + 0)
			//0->0 1->1 2->1 3->2
			#define quadTransform03(x) (0.1+(0.6*x))
			int np1 = round(heightTest(h1, p1));
			int np2 = round(heightTest(h2, p2));
			int np3 = round(heightTest(h3, p3));
			int np4 = round(heightTest(h4, p4));
			
			setTheme(w->tileCoordinate(x, y), np1);
			setTheme(w->tileCoordinate((x+1), y), np2);
			setTheme(w->tileCoordinate((x+1), (y+1)), np3);
			setTheme(w->tileCoordinate(x, (y+1)), np4);
		} else {
			
		//Hilly
		//if the chunk is not flat and doesn't have any tile with height > SEA_LEVEL + HILL_MAX*HEIGHT_RADIUS
		int hilly = (h1 > (SEA_LEVEL + (HILL_MAX * HEIGHT_RADIUS)));
		hilly |= (h2 > (SEA_LEVEL + (HILL_MAX * HEIGHT_RADIUS)));
		hilly |= (h3 > (SEA_LEVEL + (HILL_MAX * HEIGHT_RADIUS)));
		hilly |= (h4 > (SEA_LEVEL + (HILL_MAX * HEIGHT_RADIUS)));
		if (hilly != 1) {
			//-HEIGHT_RADIUS to any tiles > SEA_LEVEL + HILL_DAMPENER*HEIGHT_RADIUS
			if (h1 > (SEA_LEVEL + (HILL_DAMPENER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, y), (h1-HEIGHT_RADIUS));
			}
			if (h2 > (SEA_LEVEL + (HILL_DAMPENER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), y), (h2-HEIGHT_RADIUS));
			}
			if (h3 > (SEA_LEVEL + (HILL_DAMPENER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), (y+1)), (h3-HEIGHT_RADIUS));
			}
			if (h4 > (SEA_LEVEL + (HILL_DAMPENER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, (y+1)), (h4-HEIGHT_RADIUS));
			}
			
			//0->0 1->2 2->2 3->3
			#define quadTransform00(x) ((-0.25)*(x*x) + (1.65)*(x) + 0.15)
			int np1 = round(quadTransform00(p1));
			int np2 = round(quadTransform00(p2));
			int np3 = round(quadTransform00(p3));
			int np4 = round(quadTransform00(p4));
			
			setTheme(w->tileCoordinate(x, y), np1);
			setTheme(w->tileCoordinate((x+1), y), np2);
			setTheme(w->tileCoordinate((x+1), (y+1)), np3);
			setTheme(w->tileCoordinate(x, (y+1)), np4);
			
		} else {
		
		//Mountainous
		//if the chunk is not hilly
			
			//+HEIGHT_RADIUS to any tiles < SEA_LEVEL + MOUNTAIN_MIN_BOOSTER*HEIGHT_RADIUS, twice
			if (h1 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, y), (h1+HEIGHT_RADIUS));
			}
			if (h2 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), y), (h2+HEIGHT_RADIUS));
			}
			if (h3 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), (y+1)), (h3+HEIGHT_RADIUS));
			}
			if (h4 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, (y+1)), (h4+HEIGHT_RADIUS));
			}
			
			if (h1 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, y), (h1+HEIGHT_RADIUS));
			}
			if (h2 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), y), (h2+HEIGHT_RADIUS));
			}
			if (h3 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), (y+1)), (h3+HEIGHT_RADIUS));
			}
			if (h4 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, (y+1)), (h4+HEIGHT_RADIUS));
 			}
			
			//0->0 1->2 2->3 3->3
			#define quadTransform01(x) ((-0.5)*(x*x) + (2.5)*(x))
			int np1 = quadTransform01(p1);
			int np2 = quadTransform01(p2);
			int np3 = quadTransform01(p3);
			int np4 = quadTransform01(p4);
			
			setTheme(w->tileCoordinate(x, y), np1);
			setTheme(w->tileCoordinate((x+1), y), np2);
			setTheme(w->tileCoordinate((x+1), (y+1)), np3);
			setTheme(w->tileCoordinate(x, (y+1)), np4);
 		} //End of hill's else
		} //End the flat's else
		} //End the water's else
	}}
}

void normalizeChunks(WORLD* w) {
	int x,y,t1,t2,t3,t4,h1,h2,h3,h4;
	for (x = 0; x < W_WIDTH-1; x += 2) {
		for (y = 0; y < W_HEIGHT; y += 2) {
		
		//Catch the last row
		if (y == W_HEIGHT-1)
			y--;
			
		t1 = w->tileCoordinate(x,y);
		t2 = w->tileCoordinate((x+1), y);
		t3 = w->tileCoordinate((x+1), (y+1));
		t4 = w->tileCoordinate(x, (y+1));
		
		h1 = getHeight(t1);
		h2 = getHeight(t2);
		h3 = getHeight(t3);
		h4 = getHeight(t4);
		
		int water = (h1 <= SEA_LEVEL);
 		water += (h2 <= SEA_LEVEL);
 		water += (h3 <= SEA_LEVEL);
 		water += (h4 <= SEA_LEVEL);
		//Water
		//if the chunk is half water
		if (water > 1) {
			//Leave it alone
// 			sink(w, x, y);
// 			sink(w, x+1, y);
// 			sink(w, x+1, y+1);
// 			sink(w, x, y+1);
			setTheme(w->tileCoordinate(x, y), WATER);
			setTheme(w->tileCoordinate((x+1), y), WATER);
			setTheme(w->tileCoordinate((x+1), (y+1)), WATER);
			setTheme(w->tileCoordinate(x, (y+1)), WATER);
		} else {
		
		//Flat land
		//if the chunk isn't water and doesn't have any tile with height > SEA_LEVEL + (FLAT_MAX*HEIGHT_RADIUS)
		int flat = (h1 < (SEA_LEVEL + (FLAT_MAX * HEIGHT_RADIUS)));
		flat &= (h2 < (SEA_LEVEL + (FLAT_MAX * HEIGHT_RADIUS)));
		flat &= (h3 < (SEA_LEVEL + (FLAT_MAX * HEIGHT_RADIUS)));
		flat &= (h4 < (SEA_LEVEL + (FLAT_MAX * HEIGHT_RADIUS)));
		if (flat) {
			//sink each tile, than raise it?
			//...or i guess nothing?
			//setHeight(w->tileCoordinate(x, y), (h1*HEIGHT_RADIUS));
			//setHeight(w->tileCoordinate((x+1), y), (h2*HEIGHT_RADIUS));
			//setHeight(w->tileCoordinate((x+1), (y+1)), (h3*HEIGHT_RADIUS));
			//setHeight(w->tileCoordinate(x, (y+1)), (h4*HEIGHT_RADIUS));

			//sink(w, x, y);
			//sink(w, x+1, y);
			//sink(w, x+1, y+1);
			//sink(w, x, y+1);
			
			//raise(w, x, y);
			//raise(w, x+1, y);
			//raise(w, x+1, y+1);
			//raise(w, x, y+1);
			setTheme(w->tileCoordinate(x, y), FLAT);
			setTheme(w->tileCoordinate((x+1), y), FLAT);
			setTheme(w->tileCoordinate((x+1), (y+1)), FLAT);
			setTheme(w->tileCoordinate(x, (y+1)), FLAT);
		} else {
			
		//Hilly
		//if the chunk is not flat and doesn't have any tile with height > SEA_LEVEL + HILL_MAX*HEIGHT_RADIUS
		int hilly = (h1 > (SEA_LEVEL + (HILL_MAX * HEIGHT_RADIUS)));
		hilly |= (h2 > (SEA_LEVEL + (HILL_MAX * HEIGHT_RADIUS)));
		hilly |= (h3 > (SEA_LEVEL + (HILL_MAX * HEIGHT_RADIUS)));
		hilly |= (h4 > (SEA_LEVEL + (HILL_MAX * HEIGHT_RADIUS)));
		if (hilly != 1) {
			//-HEIGHT_RADIUS to any tiles > SEA_LEVEL + HILL_DAMPENER*HEIGHT_RADIUS
			if (h1 > (SEA_LEVEL + (HILL_DAMPENER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, y), (h1-HEIGHT_RADIUS));
			}
			if (h2 > (SEA_LEVEL + (HILL_DAMPENER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), y), (h2-HEIGHT_RADIUS));
			}
			if (h3 > (SEA_LEVEL + (HILL_DAMPENER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), (y+1)), (h3-HEIGHT_RADIUS));
			}
			if (h4 > (SEA_LEVEL + (HILL_DAMPENER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, (y+1)), (h4-HEIGHT_RADIUS));
			}
			
			setTheme(w->tileCoordinate(x, y), HILLS);
			setTheme(w->tileCoordinate((x+1), y), HILLS);
			setTheme(w->tileCoordinate((x+1), (y+1)), HILLS);
			setTheme(w->tileCoordinate(x, (y+1)), HILLS);
			
		} else {
		
		//Mountainous
		//if the chunk is not hilly
			
			//+HEIGHT_RADIUS to any tiles < SEA_LEVEL + MOUNTAIN_MIN_BOOSTER*HEIGHT_RADIUS, twice
			if (h1 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, y), (h1+HEIGHT_RADIUS));
			}
			if (h2 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), y), (h2+HEIGHT_RADIUS));
			}
			if (h3 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), (y+1)), (h3+HEIGHT_RADIUS));
			}
			if (h4 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, (y+1)), (h4+HEIGHT_RADIUS));
			}
			
			if (h1 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, y), (h1+HEIGHT_RADIUS));
			}
			if (h2 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), y), (h2+HEIGHT_RADIUS));
			}
			if (h3 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate((x+1), (y+1)), (h3+HEIGHT_RADIUS));
			}
			if (h4 < (SEA_LEVEL + (MOUNTAIN_MIN_BOOSTER * HEIGHT_RADIUS))) {
				setHeight(w->tileCoordinate(x, (y+1)), (h4+HEIGHT_RADIUS));
 			}
			
			setTheme(w->tileCoordinate(x, y), MOUNTAIN);
			setTheme(w->tileCoordinate((x+1), y), MOUNTAIN);
			setTheme(w->tileCoordinate((x+1), (y+1)), MOUNTAIN);
			setTheme(w->tileCoordinate(x, (y+1)), MOUNTAIN);
 		} //End of hill's else
		} //End the flat's else
		} //End the water's else
	}} //End the two fors
	
	subTile(w, 0, 1);
	//subTile(w, 1, 0);
	subTile(w, 1, 1);
	//subTile(w, 0, 0);
}

void peaksAndWater(WORLD* w) {
	int a,b,c;
	tripleFor(a,b,c) {
		TILE t = w->tileRelative(a,b,c);
		int h = getHeight(t);
		
		if ((h > SEA_LEVEL) & (getTheme(t) == WATER)) {
			//h = SEA_LEVEL - (h - SEA_LEVEL);
			//setHeight(t, h);
			setTheme(t, FLAT);
		}
		
		if (h <= SEA_LEVEL) {
			if (getTheme(t) == FLAT) {
				setHeight(t, (h + (int)(0.75*HEIGHT_RADIUS)));
			} else {
				setTheme(t, WATER);
			}
		}
		
		if (h > SEA_LEVEL + (PEAK_HEIGHT*HEIGHT_RADIUS)) {
			setTheme(t, PEAK);
		}
		
		w->tileRelative(a,b,c) = t;
	}}}
}


void themeInit(WORLD* w) {
	//Using 2x2 a squares as bases, let's come up with basic elevation themes
	normalizeChunks(w);
	peaksAndWater(w);
}