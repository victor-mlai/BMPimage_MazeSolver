# MazeGenerator_fromBMP
## Warning! Stroboscopic Output! Comment the 2 lines in function 
myBFS::printPath() to show the maze directly solved!

Bmp formats supported: 8, 16, 24, 32 bits per pixel

### Input:
```
	Add your bmp image in the input folder and then choose it from inside
the program.
	Select image (Press 1, 2, 3.. or type your own image's name)
	The columns are numerotated 1, 3, 5 due to space reasons
	Write how many exits the maze has.
	For each exit, write the exit' position.
	To have a better look on the row/column indexes, you can choose to display
a crop of the maze. 
	Write the center position and the Radius (Rectangles' height and width).
	Do the same for the Start Position.
```

### Output:
```
	The image you selected will be saved in the output directory with the name
	"<originalName>Solved.bmp"
```

#### exit/start Recomendations:
```
maze1.bmp: 
exit: 1 30
start: 29 1

maze2.bmp:
exit: 56 0
start: 13 20

maze3.bmp:
exit: 73 93
start: 9 9

1024.bmp:
exit: 1021 1023
start: 0 2

cat.bmp:
exit: 230 600
start: 20 70

theseus.bmp:
exit: 400 530
start: 740 510
```

### Controls:
```
if the dimensions of the maze is too big then I suggest maximizing the command
window and reducing the font.
Right Click on Console -> Properties -> Font -> Size -> 8
For each exit specify if you want to display a crop of maze (usefull for very
big mazes) and then type the specific parameters.
Same for the Start Point.
```

### Implementation:
```
It reads the image in binary mode, bit by bit. It takes the image's height,
widht, offset, bpp from the file's header.
And then checks every pixel how white or black is and creates acordingly a
matrix with 0s and -1s where

-2 = the positions to be colored
-1 = wall
0 = way
1 = going left
2 = going down
3 = right
4 = up
5 = exits
[6...] = the distance to the closest exit.

After giving him some Start and End points, it searches for the path using bfs
After showing the solution it creates a new BMP file using the bits from the
first one, also marking the path (colouring it in gray which is the value of
white / 2).
```

### Note:
```
if the dimensions of the maze is too big then I suggest maximizing the command
window and reducing the font.
Right Click on Console -> Properties -> Font -> Size -> 8
```

### TODO:
```
1. make the "interface" more user-friendly
2. replace printf/cout with something faster to remove the stroboscopic effect
```
