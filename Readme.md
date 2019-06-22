# How to compile:

Obs. opencv4 and qt4 required to compile the code


cd Visualizer/Visualizer

qmake Visualizer.pro

make


# How to use:

given an image with the obstacles: enviroment.png

cd Visualizer/Visualizer

./Visualizer enviroment.png

In the interface, select the desired terminal points with the right button of the mouse.

The tree is generated after clicking in generate tree.

The experiment is saved in experiments folder after clicking to save it:

The output files: steinerData1.dat, steinerData3.dat

The steinerData3.dat contains some information about the tree such as its size

The steinerData1.dat contains a graph represented in a adjacency list

ex:

0

1 4 -1

1

0 2 3 -1

2

1 -1

3

1 -1

4

0 -1

-2

0 423 138 1 460 163 2 448 350 3 659 67 4 325 83 -2


In the example above, the tree has 5 nodes (0 to 4). The node 0 is conected to node 1 and 4, node 1 is conected to 0, 2, 3 ....
The last line contains the nodes positions in the image, ex: node 0 is at (423,138)








