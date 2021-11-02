all: octo

octo: octo.cpp
	g++  octo.cpp -std=c++11 -o octo -lglfw -lglew  -framework OpenGL
