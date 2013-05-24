#include "map code/world.h"
#include "ncurses/nhelp.h"

#include <string.h>


int graduatedBackColor(int s, int min, int step) {
	if (s < (min + (3*step)))
		return COLOR_BLACK;
	else if (s < (min + (12*step)))
		return COLOR_BLUE;
	else if (s < (min + (21*step)))
		return COLOR_CYAN;
	else if (s < (min + (30*step)))
		return COLOR_GREEN;
	else if (s < (min + (39*step)))
		return COLOR_YELLOW;
	else if (s < (min + (48*step)))
		return COLOR_RED;
	else if (s < (min + (57*step)))
		return COLOR_MAGENTA;
	else
		return COLOR_WHITE;
}

int graduatedForeColor(int s, int min, int step) {
	if (s < (min + (3*step)))
		return COLOR_BLUE;
	else if (s < (min + (6*step)))
		return COLOR_BLACK;
	else if (s < (min + (9*step)))
		return COLOR_BLUE;
	else if (s < (min + (12*step)))
		return COLOR_CYAN;
	else if (s < (min + (15*step)))
		return COLOR_BLUE;
	else if (s < (min + (18*step)))
		return COLOR_CYAN;
	else if (s < (min + (21*step)))
		return COLOR_GREEN;
	else if (s < (min + (24*step)))
		return COLOR_CYAN;
	else if (s < (min + (27*step)))
		return COLOR_GREEN;	
	else if (s < (min + (30*step)))
		return COLOR_YELLOW;
	else if (s < (min + (33*step)))
		return COLOR_GREEN;
	else if (s < (min + (36*step)))
		return COLOR_YELLOW;
	else if (s < (min + (39*step)))
		return COLOR_RED;
	else if (s < (min + (42*step)))
		return COLOR_YELLOW;
	else if (s < (min + (45*step)))
		return COLOR_RED;
	else if (s < (min + (48*step)))
		return COLOR_MAGENTA;
	else if (s < (min + (51*step)))
		return COLOR_RED;
	else if (s < (min + (54*step)))
		return COLOR_MAGENTA;
	else if (s < (min + (57*step)))
		return COLOR_WHITE;
	else if (s < (min + (60*step)))
		return COLOR_MAGENTA;
	else
		return COLOR_WHITE;
}

int graduatedColor(int s, int min, int step) {
	return colorPairId(graduatedBackColor(s, min, step), graduatedForeColor(s, min, step));
}

int colorForHeight(int h) {
	if (h < SEA_LEVEL - HEIGHT_RADIUS)
		return defaultColorPairId(COLOR_BLUE);
	else if (h <= SEA_LEVEL)
		return defaultColorPairId(COLOR_CYAN);
	else if (h < SEA_LEVEL + HEIGHT_RADIUS)
		return defaultColorPairId(COLOR_GREEN);
	else if (h < SEA_LEVEL + (2*HEIGHT_RADIUS))
		return defaultColorPairId(COLOR_YELLOW);
	else if (h < SEA_LEVEL + (3*HEIGHT_RADIUS))
		return defaultColorPairId(COLOR_RED);
	else if (h < SEA_LEVEL + (4*HEIGHT_RADIUS))
		return defaultColorPairId(COLOR_MAGENTA);
	else if (h < SEA_LEVEL + (5*HEIGHT_RADIUS))
		return defaultColorPairId(COLOR_WHITE);
	else
		return defaultColorPairId(COLOR_BLACK);
}

int colorForSaturation(int s) {
	return graduatedColor(s, SAT_MIN, SAT_STEP);
// 	//Red, Yellow, Green, White, Magenta, Cyan, Blue, Black
// 	if (s <= SAT_MIN)
// 		return defaultColorPairId(COLOR_RED);
// 	else if (s < SAT_RADIUS)
// 		return defaultColorPairId(COLOR_YELLOW);
// 	else if (s < (2*SAT_RADIUS))
// 		return defaultColorPairId(COLOR_GREEN);
// 	else if (s < (3*SAT_RADIUS))
// 		return defaultColorPairId(COLOR_WHITE);
// 	else if (s < (4*SAT_RADIUS))
// 		return defaultColorPairId(COLOR_MAGENTA);
// 	else if (s < (5*SAT_RADIUS))
// 		return defaultColorPairId(COLOR_CYAN);
// 	else if (s < (6*SAT_RADIUS))
// 		return defaultColorPairId(COLOR_BLUE);
// 	else
// 		return defaultColorPairId(COLOR_BLACK);
}

int colorForTemp(TILE t) {
	if (getHeight(t) <= SEA_LEVEL)
		return colorPairId(COLOR_BLACK, COLOR_BLACK);
		
	//Hot <----> Cold	
	//White, Magenta, Red, Yellow, Green,  Cyan, Blue, Black
	int s = getTemp(t);
	return graduatedColor(s, TEMP_MIN, TEMP_STEP);
}

int colorForChunk(int id, int h) {
	if ((id == 0) & (h > SEA_LEVEL))
		return defaultColorPairId(COLOR_GREEN);
	if (id == 0)
		return defaultColorPairId(COLOR_CYAN);
	if (id == 3)
		return defaultColorPairId(COLOR_RED);
	if (id == 2)
		return defaultColorPairId(COLOR_YELLOW);
	if ((id == 1) & (h < SEA_LEVEL))
		return defaultColorPairId(COLOR_CYAN);
	if (id == 1)
		return defaultColorPairId(COLOR_GREEN);
	return defaultColorPairId(COLOR_WHITE);
}

void dispTile(int y, int x, TILE t, int mode) {
	int c;
	switch (mode) {
		case 0:
		default:
			//display heights as colors
			attrset(COLOR_PAIR(colorForHeight(getHeight(t))));
			mvaddch(y, x, ' ');
		break;
	
		//Display plates as color and height as letter
		//attrset(COLOR_PAIR(getPlateID(t)));
		//mvaddch(y, x, 'a'+getHeight(t));
	
		case 1:
			//Display ElevationTheme/height as color
			c = colorForChunk(getTheme(t), getHeight(t));
			attrset(COLOR_PAIR(c));
			mvaddch(y, x, 'a'+getTheme(t));
		break;
		
		
		case 2:
			//display saturation as colors
			attrset(COLOR_PAIR(colorForSaturation(getSaturation(t))));
			if (getHeight(t) < SEA_LEVEL)
				attrset(COLOR_PAIR(defaultColorPairId(COLOR_RED)));
			mvaddch(y, x, ' ');
		break;
		
		case 3:
			//display temp for color
			attrset(COLOR_PAIR(colorForTemp(t)));
			mvaddch(y, x, ACS_CKBOARD);
		break;
			
		case 4:
			//display altitude and coast
			attrset(COLOR_PAIR(colorForHeight(getHeight(t))));
			char c = ' ';
			if (getCoastFlag(t) != 0)
				c = 'a'+getCoastFlag(t);
			mvaddch(y, x, c);
		 break;
	}
}

void displayColorMode(int colorMode) {
	attrset(colorText);
	mvaddch(V_HEIGHT+1, 12, '0'+colorMode);
	printw(" - ");
	switch(colorMode) {
		default:
			printw("Altitude Map    ");
			break;
		case 1:
			printw("Themes          ");
			break;
		case 2:
			printw("Saturation Map  ");
			break;
		case 3:
			printw("Temperature Map ");
			break;
		case 4:
			printw("Coastal Map      ");
			break;
	}
}

void DisplayWorld(WORLD* w, int mode) {
	int x = 0, y = 0;
	for (x = 0; x < 80; x++) {
		for (y = 0; y < 25; y++) {
			BLOCK b = w->blockCoordinate((ZOOM_FACTOR*x), (ZOOM_FACTOR*y));
			//BLOCK b = w->views[0].blocks[0];
			dispTile(y, x, blockSummary(b), mode);
		}
	}
	refresh();
}

void DisplayRollingView(WORLD* w, int vx, int vy, int mode) {
	int x = 0, y = 0;
	for (x = 0; x < 80; x++) {
		for (y = 0; y < 25; y++) {
			int nx = (x + vx);
			nx %= W_WIDTH;
			TILE t = w->tileCoordinate(nx, (y + vy));
			dispTile(y,x,t,mode);
		}
	}
	refresh();
}

void DisplayStaticView(WORLD* w, int view, int mode) {
	POINT* viewPoint = coordinateFromRelativeIndex(view, 0, 0);
	DisplayRollingView(w, viewPoint->x, viewPoint->y, mode);
	free(viewPoint);
}

POINT* go_to() {
	attrset(colorText);
	mvprintw(V_HEIGHT + 2, 1, "x:000 y:000 ");
	int index = 0, field = 0;
	while (TRUE) {
		int g = getch();
		if ((g <= '9') & (g >= '0')) {
			mvaddch(V_HEIGHT + 1, (6*index)+(3+field), g);
			field++;
		}
	
		if (g == KEY_RIGHT) {
			field++;
		}
	
		if (g == KEY_LEFT) {
			field--;
		}
	
		if (field > 2) {
			field = 0;
			index = abs(index-1);
		}
	
		if (field < 0) {
			field = 2;
			index = abs(index-1);
		}
		
		if ((g == KEY_ENTER) | (g == ' ') | (g =='\r') | (g =='\n')) {
			char* c;
			mvinnstr(V_HEIGHT + 1, 3, c, 3);
			int x = atoi(c);
			mvinnstr(V_HEIGHT + 1, 9, c, 3);
			int y = atoi(c);
			mvprintw(V_HEIGHT + 1, 1, "            ");
			refresh();
			//return the mode from the coordinates
			return Point(x,y);
		}
		refresh();
	}
}

WORLD* loadInput(WORLD* def) {
	attrset(colorText);
	mvprintw(V_HEIGHT + 2, 1, "Load map: 0");
	int index = 0, max=1;
	while (TRUE) {
		move(V_HEIGHT + 2, 11+index);
		int g = getch();
		if ((g <= '9') & (g >= '0')) {
			addch(g);
			index++;
			if (index > max)
				max++;
		}
	
		if (g == KEY_RIGHT) {
			index++;
		}
	
		if (g == KEY_LEFT) {
			index--;
		}
	
		if (index > max) {
			index = 0;
		}
	
		if (index < 0) {
			index = max;
		}
		
		if ((g == KEY_ENTER) | (g == ' ') | (g =='\r') | (g =='\n')) {
			char* c = (char*)calloc(max+5, sizeof(char));
			mvinnstr(V_HEIGHT + 2, 11, c, max);
			
			mvprintw(V_HEIGHT + 2, 1, "           ");
			while (max-- > 0)
				addch(' ');
			refresh();
			
			//return the map
			strcat(c, ".map");
			WORLD* newW = loadWorld(c);
			if (newW == NULL) {
				mvprintw(V_HEIGHT + 2, 1, "No map found (cont)");
				getch();
				mvprintw(V_HEIGHT + 2, 1, "                   ");
				return def;
			}
			return newW;
		}
		
		if (g == 'q') {
			mvprintw(V_HEIGHT + 2, 1, "           ");
			while (max-- > 0)
				addch(' ');
			refresh();
			return def;
		}
		refresh();
	}
}

void saveInput(WORLD* w) {
	attrset(colorText);
	mvprintw(V_HEIGHT + 2, 1, "Save map: 0");
	int index = 0, max=1;
	while (TRUE) {
		move(V_HEIGHT + 2, 11+index);
		int g = getch();
		if ((g <= '9') & (g >= '0')) {
			addch(g);
			index++;
			if (index > max)
				max++;
		}
	
		if (g == KEY_RIGHT) {
			index++;
		}
	
		if (g == KEY_LEFT) {
			index--;
		}
	
		if (index > max) {
			index = 0;
		}
	
		if (index < 0) {
			index = max;
		}
		
		if ((g == KEY_ENTER) | (g == ' ') | (g =='\r') | (g =='\n')) {
			char* c = (char*)calloc(max+5, sizeof(char));
			mvinnstr(V_HEIGHT + 2, 11, c, max);
			
			mvprintw(V_HEIGHT + 2, 1, "           ");
			while (max-- > 0)
				addch(' ');
			refresh();
			
			//return the map
			strcat(c, ".map");
			saveWorld(w, c);
			return;
		}
		
		if (g == 'q') {
			mvprintw(V_HEIGHT + 2, 1, "           ");
			while (max-- > 0)
				addch(' ');
			refresh();
			return;
		}
		refresh();
	}
}

void DisplayInteractive(WORLD* w) {
	WORLD* sub = NULL;
	WORLD* temp = w;
	mvprintw(V_HEIGHT, 1, "+/-/e Zoom | Movement: Arrow Keys=Large WASD=Medium wasd=Small | q Quits");
	int mode = 1;
	int x = 0, y = 0;
	int colorMode = 0;
	POINT* p;
	while (TRUE) {
		if (y < 0)
			y = 0;
		if (y > W_HEIGHT-1)
			y = W_HEIGHT-1;
		if (x < 0)
			x += W_WIDTH;
		x %= W_WIDTH;
		displayColorMode(colorMode);
		if (mode == 0) {
			//Scroll mode
			int vy = y;
			if (vy > (W_HEIGHT-V_HEIGHT-1))
				vy = (W_HEIGHT-V_HEIGHT-1); 
			DisplayRollingView(w, x, vy, colorMode);
			move(y-vy, 0);
		} else if (mode == 1) {
			//World Mode
			DisplayWorld(w, colorMode);
			move(y/ZOOM_FACTOR, x/ZOOM_FACTOR);
		} else {
			return;
			//Zoom in on mode
			//DisplayStaticView(w, mode, colorMode);
		}
		int g = getch();
		switch(g) {
			case 'q':
				return;
			case 'l':
				sub = loadInput(sub);
				break;
			case 'm':
				saveInput(w);
				break;
			case 'x':
				if (sub == NULL)
					sub = loadWorld("0.map");
				w = sub;
				break;
			case 'z':
				w = temp;
				break;
			case '~':
			case '`':
			case '1':
			case '2':
			case '4':
			case '3':
			case '6':
			case '5':
			case '8':
			case '0':
			case '9':
			case '7':
				colorMode = g-'0';
				break;
			case '+':
				mode = 0;
				break;
			case '-':
				mode = 1;
				break;
			case 'e':
				mode = (-1)*(mode-1);
				break;
			case 'a':
				x--;
				break;
			case 'd':
				x++;
				break;
			case 'w':
				y--;
				break;
			case 's':
				y++;
				break;
			case 'A':
				x -= ZOOM_FACTOR;
				break;
			case 'D':
				x += ZOOM_FACTOR;
				break;
			case 'W':
				y -= ZOOM_FACTOR;
				break;
			case 'S':
				y += ZOOM_FACTOR;
				break;
			case KEY_LEFT:
				x -= V_WIDTH;
				break;
			case KEY_RIGHT:
				x += V_WIDTH;
				break;
			case KEY_UP:
				y -= V_HEIGHT;
				break;
			case KEY_DOWN:
				y += V_HEIGHT;
				break;
			case 'g':
				p = go_to();
				mode = 0;
				x = p->x;
				y = p->y;
				free(p);
				break;
			case 'r':
				w = newWorldRandom();
				break;
		}
	}
}

int main(int argc, char** argv) {
	printf("Please wait...\n");
	WORLD* w;
	if (argc == 2) {
		w = newWorld(atoi(argv[1]));
	} else {
		w = newWorldRandom();
	}
	
	n_engage();
// 	init_pair(1, COLOR_BLACK, COLOR_RED);
// 	init_pair(2, COLOR_BLACK, COLOR_YELLOW);
// 	init_pair(3, COLOR_BLACK, COLOR_GREEN);
// 	init_pair(4, COLOR_WHITE, COLOR_CYAN);
// 	init_pair(5, COLOR_WHITE, COLOR_BLUE);
// 	//testing
// 	init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
// 	init_pair(7, COLOR_BLACK, COLOR_WHITE);
// 	init_pair(8, COLOR_WHITE, COLOR_BLACK);

	DisplayInteractive(w);
	n_disengage();
	return 0;
}

