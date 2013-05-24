Map Code
========

[Header File](world.h)
----------------------

This header file defines a lot of helper constants and macros expected to be used to refer to the world. It defines the size of a TILE, and how to store and remove different types of information in the TILE (Done through a mask). 

A world is made up of a number of views, each with a number of blocks, each with a number of TILEs. These numbers were chosen so when the map is "zoomed out", we can summarize the 25 TILEs in each block into a single TILE, which will then all be displayed in a 80x25 terminal screen. When we "zoom in", we then just see a view's worth of TILEs.

The View Width/Height (V_WIDTH/V_HEIGHT) and the zoom factor (ZOOM_FACTOR), then determine three variables, A, B, and C, which then define a world structure as w.views[A].blocks[B].tiles[C]. This triplet (A,B,C) is defined as the TILE's relative location, and can be access through a macro as w.tileRelative(a,b,c) (or when w is a WORLD* w->tileRelative(a,b,c)).

In addition to the tile's relative location, each tile has an (x, y) coordinate that can be calcuated with it's relative coordinate. (0,0,0) corresponds to (0,0), and X increases going right, and Y increases going down.

[World Utility](worldutil.c)
----------------------------

Utility functions, useful to the Display code and multiple other files in world creation.

[World Generation](world.c)
---------------------------

This file contains several debugging functions (prefaced with dbg_) and the "main" function that creates the world. The newWorld function sends the task off to other methods, most of which are in other files.  The first method is in this file still, and just adds a bit of noise, so our base landscape is more interesting than flat land at 0.

[Plates for Techtonics](plates.c)
---------------------------------

This file contains an implementation of Voronoi tiles, which are used a couple a times in map creation, mainly as the plates for Plate Techtonics.

[Plate Techtonics](techtonics.c)
--------------------------------

This is the second method's file, and take some voronoi tiles and pretends they're plates for plate techtonics. Some plates go over, some plates go under, and mountain ranges and oceans are formed.

[Theme Inititalization](tileinit.c)
-----------------------------------

This contains the third method. Themes are what will eventually define what kind of physical feature a TILE represents. This initializes a few basic Themes; Water, Flatland, Hills, Mountains, and Peaks. When displayed, they are represented by color.

[Temperature Inititalization](temp.c)
-------------------------------------

This contains the fourth and sixth methods. The fourth method initializes tempuratures based on latitude, ie it gets hotter towards the equator. The sixth method averages each TILE's temperature based on the surrounding temperatures.

[Costal Climates](coastal.c)
----------------------------

The fifth method identifies the coasts of continents, and modifies the temperature of them as if affected by coastal winds. For more information, look up Hadley cells or other climate information.

[Water/Saturation Distribution](waterdist.c)
--------------------------------------------

The seventh step would distribute water, as in rainfall, which would identify deserts and such. This step, however, is unfinished and does not give good results.
