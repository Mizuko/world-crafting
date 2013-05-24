void tempInit(WORLD* w) {
	int x,y;
	doubleFor(x,y) {
		TILE tile = w->tileCoordinate(x,y);
		if (getHeight(tile) <= SEA_LEVEL)
			continue;
		
		int d = y;
		if (y > (W_HEIGHT/2)) {
			d = W_HEIGHT - y;
		}
		
		int t = TEMP_MIN + (int)(d*TEMP_STEP);
		setTemp(tile, t);
		w->tileCoordinate(x,y) = tile;
	}}	
}

void tempBlur(WORLD* w) {
	WORLD* buffer = (WORLD*)malloc(sizeof(WORLD));
	int x,y;
	for(x=0;x<W_WIDTH;x++){
	for(y=2;y<W_HEIGHT-2;y++){
		TILE t = w->tileCoordinate(x,y);
		TILE* adj = adjacentTiles(w, x, y);
		
		int temp = getTemp(t);
		int mod = 0;
		int count = 1;
		if ((adj[0] != 0) & (getHeight(adj[0]) > SEA_LEVEL)) {
			count++;
			mod += getTemp(adj[0]) - temp;
		}
		if ((adj[1] != 0) & (getHeight(adj[1]) > SEA_LEVEL)) {
			count++;
			mod += getTemp(adj[1]) - temp;
		}
		if ((adj[2] != 0) & (getHeight(adj[2]) > SEA_LEVEL)) {
			count++;
			mod += getTemp(adj[2]) - temp;
		}
		if ((adj[3] != 0) & (getHeight(adj[3]) > SEA_LEVEL)) {
			count++;
			mod += getTemp(adj[3]) - temp;
		}
		free(adj);
		
		temp += (mod/count);
		setTemp(t, temp);
		buffer->tileCoordinate(x, y) = t;
	}}
	
	doubleFor(x,y) {
		TILE t = w->tileCoordinate(x,y);
		setTemp(t, getTemp(buffer->tileCoordinate(x,y)));
		w->tileCoordinate(x,y) = t;
	}}
	
	free(buffer);
}
