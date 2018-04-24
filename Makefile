OBJS=main.cpp
OBJ_NAME=a.out
all:
	g++ $(OBJS) -w -lSDL2 -lSDL2_image -o $(OBJ_NAME) 
