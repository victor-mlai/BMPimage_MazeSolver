# MazeGenerator_fromBMP
Bmp formats supported: 8, 16, 24, 32 bits per pixel

Input: in main() please specify input image name
same for Output file.

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

TODO:
1. solve the 1024.bmp image
