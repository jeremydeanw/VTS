# IMPORTANT: Change MacFramework to LinuxFramework when necessary.

CC = g++

ExeName = pt2277_t05m01

Bezier = BezierUtils.cpp
GLScene = OpenGLScene.cpp 
Main = main.cpp

MacFramework = -framework Carbon -framework OpenGL -framework GLUT
LinuxFramework = -lglut -lGLU

all: main

main:
	$(CC) -g -o $(ExeName) $(Main) $(Bezier) $(GLScene) $(MacFramework)

clean:
	rm -f $(ExeName)
