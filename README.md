world-crafting
==============

A program that attempts to model a Earth-like world's continents and climates.
Currently, the Saturation Map is under construction.
Also, a known bug is that a select few map seeds will cause a segmentation fault. These seeds are few and far between, so I haven't sought them out to fix them.

The code uses ncurses for terminal display. It works best if your terminal can display color and is at least 80x30. The map is shown on the first 25 lines, the last five are for the interface, like controls explanation.

The program is heavily split into parts. Code that [generates the map](./map code), [ncurses support](./ncurses), and other [utilities](./util), with the map code being subdivided into files from there.

The command I use to compile this mess is found in the batch file comp.
The resulting program, once complied, may either be run "./disp" for a random world, or "./disp seed" for a world based on the seed.
