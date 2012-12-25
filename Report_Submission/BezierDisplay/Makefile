# IMPORTANT: Change MacFramework to LinuxFramework when necessary.

CC = g++

ExeName = vtsynth

MacOpenGL = -framework GLUT -framework OpenGL
LinuxOpenGL = -lglut -lGLU

Include = -I./include/eigen/ -I./Src/ -I./Src/OpenGL/ -I./Src/PolyBezier
Flags = -g
Sources = ./Src/*.cpp ./Src/OpenGL/*.cpp ./Src/PolyBezier/*.cpp

all: main

main:
	$(CC) -o $(ExeName) $(Include) $(MacOpenGL) $(Flags) $(Sources)

%.o : %.cpp
	$(CC) -c $(Include) $(MacOpenGL) $(Flags) $(ExeName) $<

clean:
	rm -f $(ExeName)
