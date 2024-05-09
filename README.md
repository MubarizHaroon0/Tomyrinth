# Tomyrinth
Enchanted Maze Explorer 
![highscore](https://github.com/MubarizHaroon0/Tomyrinth/assets/161754323/54bcf0f2-7f0f-49cb-9076-b54f7f492891)

Here's a refined version of your description with corrected grammar:

# Description:

A maze game built with C++ using the Standard Fast Multimedia Library.

# How to Play:

The objective is to reach the top-right corner while avoiding Tom. For help, you can press 'S' twice to view the path, 'E' to exit, and use the arrow keys for movement.
# Docker 
docker pull mubarizharoon/mazeexplorer .

docker run -it --rm -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix tomyrinth
# Working
Download SFML   : https://www.sfml-dev.org/download/sfml/2.6.1/

Compile : g++ main.cpp -o me -lsfml-graphics -lsfml-window -lsfml-system

Run : ./me
