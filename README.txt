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
A very basic ocean/water simulation basing.
There are 3 types of waves : Directional, circular and ripple.
For more detail, you can read the pdf in doc/
There also some phong shading and camera control.

Problems:
Currently, We did not test the program on Linux.
We only tested on Windows and MacOSX.