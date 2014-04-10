# makefile for LunarLanderX project
# compile and run using :
# $make run
NAME = "LunarLanderX"
SOURCE1 = "main.cpp"
SOURCE2 = "mist.cpp"
SOURCE3 = "loadbitmap.cpp"
SOURCE4 = "handle.cpp"
SOURCE5 = "story.cpp"

all:
	@echo "Compiling..."
	g++ -o $(NAME) $(SOURCE1) $(SOURCE2) $(SOURCE3) $(SOURCE4) $(SOURCE5) -L/usr/X11R6/lib -lX11 -lstdc++

run: all
	@echo "Running..."
	./$(NAME) 
