# MazeGenerator_fromBMP
Warning! Stroboscopic Output! Comment in function tipar() to show the maze directly solved!

Bmp formats supported: 8, 16, 24, 32 bits per pixel

Input: in main() please specify input image name
same for Output file.

Controls:
In console:
	Select image ( Press 1, 2, 3.. or type your own image's name )
	Wait until it loads it then press any key to show the maze
	The columns are numerotated 1, 3, 5 due to display space reasons
	
if the dimensions of the maze is too big then I suggest maximizing the command window and reducing the font.
Right Click -> Properties -> Font -> Size -> 8
For each exit specify if you want to display a crop of maze (usefull for very big mazes) and then type the specific parameters.
Same for the Start Point.

Implementation:
It reads the image in binary mode, bit by bit. It takes the image's height, widht, offset, bpp from the file's header.
And then checks every pixel how white or black is and creates acordingly a matrix with 0s and -1s where
0 = way
-1 = wall.
1 = going left
2 = going down
3 = right
4 = up
5 = exits
[6...] = the distance to the closest exit.
After giving him some Start and End points, it searches for the path using either bfs or backtracking (user's choice).
After showing the solution it creates a new BMP file using the bits from the first one, also marking the path 
(colouring it in gray which is the value of white / 2).

Note, Backtracking is very slow, I suggest sticking to the first option.
2nd Note: if the dimensions of the maze is too big then I suggest maximizing the command window and reducing the font.
Right Click -> Properties -> Font -> Size -> 8

TODO:
1. make "interface" more user-friendly
