
void initCoastMap(WORLD* w) {
	int x,y;
	doubleFor(x,y) {
		TILE t0 = w->tileCoordinate(x,y);
		if (getHeight(t0) > SEA_LEVEL) {
			TILE* adj = adjacentTiles(w, x, y);
			
			if ((adj[0] != 0) & (getHeight(adj[0]) <= SEA_LEVEL)) {
				setCoastFlag(t0, 1);
			} else if ((adj[1] != 0) & (getHeight(adj[1]) <= SEA_LEVEL)) {
				setCoastFlag(t0, 1);
			} else if ((adj[2] != 0) & (getHeight(adj[2]) <= SEA_LEVEL)) {
				setCoastFlag(t0, 1);
			} else if ((adj[3] != 0) & (getHeight(adj[3]) <= SEA_LEVEL)) {
				setCoastFlag(t0, 1);
			}
			
			w->tileCoordinate(x,y) = t0;
		}
	}}
}

void heatCoast(WORLD* w, int x, int y, int direction, int fullTemp, int power) {
	if (power <= 0)
		return;
	
	TILE t = w->tileCoordinate(x,y);
	power -= ((getHeight(t)-SEA_LEVEL)*COAST_TEMP_HEIGHT_RATIO);
	
	int rt = fullTemp;
	if (power < 100)
		rt *= ((double)power/100.0);
	
	int temp = getTemp(t);
	temp += rt;
	if (temp < TEMP_MIN)
		temp = TEMP_MIN;
	
	if (temp > TEMP_MAX)
		temp = TEMP_MAX;
	
	setTemp(t, temp);
	w->tileCoordinate(x,y) = t;
	
	x += direction + W_WIDTH;
	x %= W_WIDTH;
	
	heatCoast(w, x, y, direction, fullTemp, power);
}

// void rainfall(WORLD* w, int x, int y, int direction, int fullRain, int power) {
// 	if (power <= 0)
// 		return;
// 	
// 	TILE t = w->tileCoordinate(x,y);
// 	power -= ((getHeight(t)-SEA_LEVEL)*COAST_RAIN_HEIGHT_RATIO);
// 	
// 	int rt = fullRain;
// 	if (power < 100)
// 		rt *= ((double)power/100.0);
// 	
// 	int rain = getSaturation(t);
// 	rain += rt;
// 	setSaturation(t, rain);
// 	w->tileCoordinate(x,y) = t;
// 	
// 	x += direction + W_WIDTH;
// 	x %= W_WIDTH;
// 	
// 	rainfall(w, x, y, direction, fullRain, power);
// }

void coastalCurrent(WORLD* w, POINT* start, int id, POINT* end) {
	if (end->y == start->y)
		return;
		
	int dx = (start->x + (W_WIDTH-1))%W_WIDTH;
	int landDir= 0;
	TILE t = w->tileCoordinate(dx, start->y);
	if (getHeight(t) <= SEA_LEVEL)
		landDir = 1;
	else
		landDir = -1;
		
	int right = end->x - start->x;
	if (right == 0)
		right = landDir;
	else if (right > 0)
		right = 1;
	else
		right = -1;
	
	int hot = 0;
	if (start->y <= (W_HEIGHT/2)) {
		//Northern Hemi
		if (right > 0)
			hot = -1;
		else
			hot = 1;
	} else {
		//Southern Hemi
		if (right > 0)
			hot = 1;
		else
			hot = -1;
	}
	
	int x,y,temp,power;
	x = start->x;
	y = start->y;
	temp = hot*COAST_TEMP_MOD;
	power = COAST_TEMP_POWER;
	while (1) {
		if (end->y == y)
			break;
		heatCoast(w, x, y, landDir, temp, power);
		
		y++;
		TILE t = w->tileCoordinate(x, y);
		if (getCoastFlag(t) == id)
			continue;
		
		x = (W_WIDTH + x - 1)%W_WIDTH; 
		t = w->tileCoordinate(x, y);
		if (getCoastFlag(t) == id)
			continue;
		
		x = (x + 2)%W_WIDTH;
	}
}

POINT* traceCoast(WORLD* w, int x, int y, int id) {
	TILE t = w->tileCoordinate(x, y);
	setCoastFlag(t, id);
	w->tileCoordinate(x,y) = t;
	
	if (y >= W_HEIGHT - POLE_WIDTH)
		return Point(x,y);
		
	int dx,dy;
	dx = x;
	dy = y + 1;
	
	t = w->tileCoordinate(dx, dy);
	if (getCoastFlag(t) == 1)
		return traceCoast(w, dx, dy, id);
		
	dx = (W_WIDTH + x - 1)%W_WIDTH; 
	t = w->tileCoordinate(dx, dy);
	if (getCoastFlag(t) == 1)
		return traceCoast(w, dx, dy, id);
		
	dx = (x + 1)%W_WIDTH; 
	t = w->tileCoordinate(dx, dy);
	if (getCoastFlag(t) == 1)
		return traceCoast(w, dx, dy, id);
		
	return Point(x,y);
}

void coastFindAndMod(WORLD* w) {
	int x,y,id=2;
	for(y=POLE_WIDTH;y<W_HEIGHT-POLE_WIDTH;y++){
	for(x=0;x<W_WIDTH;x++){
		TILE t = w->tileCoordinate(x, y);
		if (getCoastFlag(t) != 1)
			continue;
		
		POINT* start = Point(x,y);
		POINT* end = traceCoast(w, x, y, id);
		coastalCurrent(w, start, id, end);
		id++;
		if (id >= 16)
			id = 2;
	}}
}

void coastalMods(WORLD* w) {
	initCoastMap(w);
	coastFindAndMod(w);
}