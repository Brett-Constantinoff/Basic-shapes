all: shapes

shapes: shapes.cpp
	g++  shapes.cpp -std=c++11 -o shapes -lglfw -lglew  -framework OpenGL
