all: cube

cube: cube.cpp
	g++  cube.cpp -std=c++11 -o cube -lglfw -lglew  -framework OpenGL
