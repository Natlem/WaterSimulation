Requirements:
	- Cmake
	- Opengl (>= 3.3)
	- g++ (>= 4.8) (c++11)
	

How to compile this project:
mkdir build
cd build
cmake ..
make
cd bin
./syn mesh 1

Summary:
A very basic ocean/water simulation based on GPU GEMS Chapter 1.
There are 3 types of waves : Directional, circular and ripple.
There also some phong shading and camera control.

Problems:
On Linux, the rendering is "kinda" broken
