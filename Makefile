all:
	g++ Object.cpp main.cpp Camera.cpp Shader.cpp -o objloader -I/usr/include/GL -lglut -lGL -lGLEW -lSOIL -std=c++11
