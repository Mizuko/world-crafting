#include <math.h>
//Distributes the water to the peopls

void waterClose(WORLD* w, WORLD* u) {
	int a,b,c;
	tripleFor(a,b,c) {
		int h = getSaturation(u->tileRelative(a,b,c));
		setSaturation(w->tileRelative(a,b,c), (int)(sqrt(h)));
	}}}
}

int spreadWater(WORLD* w) {
	int total = 0;
	int x,y;
	doubleFor(x,y) {
		TILE t = w->tileCoordinate(x,y);
		if (getSaturation(t) == 0)
			continue;
		if (getSaturation(t) == 255)
			continue;
		//printf("(%d %d) ; %lu ; %lu\n", x, y, getHeight(t), getSaturation(t));
		//-0-
		//1o2
		//-3-
		int* priorities = (int*)calloc(sizeof(int), 4);
		int choice = -1;
		#define priority(t, a) (((getSaturation(a) < getSaturation(t)))*((int)getHeight(t) - (int)getHeight(a)))
		#define testPriority(choice, this) if(priorities[choice] < priorities[this]) choice = this
		if (x > 0) {
			TILE a = w->tileCoordinate((x-1), y); 
			priorities[1] = priority(t, a);
			choice = 1;
			//printf("   %d: (%d %d) ; %lu ; %lu = %d\n", 1, x-1, y, getHeight(a), getSaturation(a), priority(t, a));
		}
		
		if (x < W_WIDTH-1) {
			TILE a = w->tileCoordinate((x+1), y); 
			priorities[2] = priority(t, a);
			testPriority(choice, 2);
			//printf("   %d: (%d %d) ; %lu ; %lu = %d\n", 2, x+1, y, getHeight(a), getSaturation(a), priority(t, a));
		}
		
		if (y > 0) {
			TILE a = w->tileCoordinate(x, (y-1)); 
			priorities[0] = priority(t, a);
			testPriority(choice, 0);
			//printf("   %d: (%d %d) ; %lu ; %lu = %d\n", 0, x, y-1, getHeight(a), getSaturation(a), priority(t, a));
		}
		
		if (y < W_HEIGHT-1) {
			TILE a = w->tileCoordinate(x, (y+1)); 
			priorities[3] = priority(t, a);
			testPriority(choice, 3);
			//printf("   %d: (%d %d) ; %lu ; %lu = %d\n", 3, x, y+1, getHeight(a), getSaturation(a), priority(t, a));
		}
		
		//printf("   %d = %d\n", choice, priorities[choice]);
		if (priorities[choice] < 1) {
			//printf("   (%d %d) ; %lu -> %lu\n", x, y, getSaturation(t), getSaturation(t)+1);
			setSaturation(w->tileCoordinate(x,y), (1+getSaturation(t)));
			total++;
		} else {
			int dx = 0,dy = 0;
			//-0-
			//1o2
			//-3-
			switch(choice) {
				case 0:
					dy = -1;
					break;
				case 1:
					dx = -1;
					break;
				case 2:
					dx = 1;
					break;
				case 3:
					dy = 1;
					break;
			}
			TILE c = w->tileCoordinate((x+dx), (y+dy));
			
			if (getHeight(c) > SEA_LEVEL) {
				//printf("   (%d %d) ; %lu -> %lu\n", x, y, getSaturation(t), getSaturation(t)-1);
				setSaturation(w->tileCoordinate(x,y), (getSaturation(t)-1));
				//printf("   (%d %d) ; %lu -> %lu\n", x+dx, y+dy, getSaturation(c), getSaturation(c)+1);
				setSaturation(w->tileCoordinate((x+dx), (y+dy)), (getSaturation(c)+1)); 
			} else {
				setHeight(w->tileCoordinate(x,y), SEA_LEVEL-1);
			}
		}
	}}
	return total;
}

int newSaturation(int* powers) {
	int n;
	for (n = 1; n < 255; n++) {
		if (powers[n] == 0) {
			return n;
		}
			
	}
}

void mergeSaturations(WORLD* w, int* powers, int one, int two, int max_x) {
	int toReplace = one, toKeep = two;
	if (powers[two] < powers[one]) {
		toReplace = two;
		toKeep = one;
	}
		
	int x,y;
	for (x = max_x; x >= 0; x--)
	for (y = W_HEIGHT-1; y >= 0; y--) {
		TILE t = w->tileCoordinate(x,y);
		if (getSaturation(t) == toReplace) {
  			setSaturation(t, toKeep);
  			w->tileCoordinate(x,y) = t;
			powers[toKeep]++;
			powers[toReplace]--;
		
			if (powers[toReplace] == 0)
				return;
		}
	}
}

int* waterPower(WORLD* w) {
	int* powers = (int*)calloc(255, sizeof(int));
	int x,y;
	for (x = 0; x < 255; x++)
		powers[x] = 0;
	
	doubleFor(x,y) {
		TILE t = w->tileCoordinate(x,y);
		if (getHeight(t) > SEA_LEVEL)
			continue;
		//We're at water
		
		TILE t0;
		int saturation = 0;
		if (y != 0) {
			t0 = w->tileCoordinate(x,(y-1));
			saturation = getSaturation(t0);
		}
		
		if ((saturation == 0) & (x != 0)) {
			t0 = w->tileCoordinate((x-1), y);
			saturation = getSaturation(t0);
		}
		
		if (saturation == 0) {
			//We need a new saturation
			saturation = newSaturation(powers);
		}
		
		powers[saturation]++;
		setSaturation(t, saturation);
		w->tileCoordinate(x,y) = t;
		
		if (x != 0) {
			t0 = w->tileCoordinate((x-1),y);
			int other = getSaturation(t0);
			
			if ((other != 0) & (saturation != other)) {
				mergeSaturations(w, powers, saturation, other, x);
			}
		}
	}}
	
	for (y = 0; y < W_HEIGHT; y++) {
		TILE t = w->tileCoordinate(0,y);
		TILE t0 = w->tileCoordinate((W_WIDTH-1),y);
		
		int s1 = getSaturation(t);
		int s0 = getSaturation(t0);
		if ((s1 != 0) & (s0 != 0) & (s1 != s0))
			mergeSaturations(w, powers, s1, s0, W_WIDTH-1); 
	}
	return powers;
}

void rainClouds(WORLD* w, int power, int x, int y, int direction);
void rainCloudsStart(WORLD* w, int power, int x, int y, int direction) {
	// 11           44
	//  1    2  3   4
	//      22  33
	int xPlus = (x+1)%W_WIDTH;
	int xMinus = (x+W_WIDTH-1)%W_WIDTH;
	switch(direction) {
		default:
		case 1:
			if (y > 0) {
				rainClouds(w, power, x, y-1, 1);
				rainClouds(w, power, xPlus, y-1, 1);
			}
			rainClouds(w, power, xPlus, y, 1);
			break;
		case 2:
			if (y < W_HEIGHT-1) {
				rainClouds(w, power, x, y+1, 1);
				rainClouds(w, power, xPlus, y+1, 1);
			}
			rainClouds(w, power, xPlus, y, 1);
			break;
		case 3:
			if (y < W_HEIGHT-1) {
				rainClouds(w, power, x, y+1, 1);
				rainClouds(w, power, xMinus, y+1, 1);
			}
			rainClouds(w, power, xMinus, y, 1);
			break;
		case 4:
			if (y > 0) {
				rainClouds(w, power, x, y-1, 1);
				rainClouds(w, power, xMinus, y-1, 1);
			}
			rainClouds(w, power, xMinus, y, 1);
			break;
	}
}

void rainClouds(WORLD* w, int power, int x, int y, int direction) {
	TILE t = w->tileCoordinate(x,y);
	if (getHeight(t) <= SEA_LEVEL)
		return;
	if (power <= 0)
		return;
	
	power -= ((getHeight(t)-SEA_LEVEL)*RAIN_HEIGHT_RATIO);
	
	int rt = RAIN_TOTAL;
	if (power < 100)
		rt *= ((double)power/100.0);

	int temp = getSaturation(t);
	if (temp > rt)
		rt = temp;
// 	temp += rt;
// 	if (temp < TEMP_MIN)
// 		temp = TEMP_MIN;
// 	
	if (rt > 255)
		rt = 255;
		
	setSaturation(t, rt);
	w->tileCoordinate(x,y) = t;
	rainCloudsStart(w, power, x, y, direction);
}

void rainfall(WORLD* w, int* powers) {
	int x,y;
	doubleFor(x,y) {
		TILE t = w->tileCoordinate(x,y);
		if (getHeight(t) > SEA_LEVEL)
			continue;
		int power = RAIN_POWER_RATIO*powers[getSaturation(t)];
		if (power > RAIN_POWER_MAX)
			power = RAIN_POWER_MAX;
		
		rainCloudsStart(w, power, x, y, 1);
		rainCloudsStart(w, power, x, y, 2);
		rainCloudsStart(w, power, x, y, 3);
		rainCloudsStart(w, power, x, y, 4);
	}}
}

void hadleyEffect(WORLD* w) {
	int x,y,equator;
	equator = EQUATOR;
	double cellHeight = equator*HADLEY_RADIUS;
	doubleFor(x,y) {
		if (y > equator) {
			//South
			if (y < (equator + cellHeight)) {
				//In da cell
				double power = 1 - ((y - equator)/cellHeight);
				int sat = (HADLEY_MAX*power);
				setSaturation(w->tileCoordinate(x,y), sat);
			}
		} else {
			//North
			if (y > (equator - cellHeight)) {
				//In da cell
				double power = 1 - ((equator - y)/cellHeight);
				int sat = (HADLEY_MAX*power);
				setSaturation(w->tileCoordinate(x,y), sat);
			}
		}
	}}
}

void waterDistribution(WORLD* w) {
	int a,b,c;
	tripleFor(a,b,c) {
		setSaturation(w->tileRelative(a,b,c), SAT_MIN);
	}}}
	
	hadleyEffect(w);
	
	//int* powers = waterPower(w);
	//rainfall(w, powers);
	//free(powers);
}