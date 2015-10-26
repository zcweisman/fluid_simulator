CC=g++
CFLAGS= -ansi -pedantic -Wno-deprecated -g -std=c++11
LIB= -lGL -lGLEW -lglfw -lXrandr -lXinerama -lXi -lXcursor -lm -ldl -lXrender -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lxcb-dri3 -lxcb-present -lxcb-sync -lxshmfence -lXxf86vm -lXfixes -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp
LIB_OSX= -framework OpenGL

all:

ubuntu:
	$(CC) $(CFLAGS) main.cpp $(LIB)

fedora:
	g++ -lGL -lGLEW -lglfw main.cpp


windows:
	echo "Not yet configured for Windows"

clean:
	rm -rf *~ *.o
