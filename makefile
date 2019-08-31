components.o: src/components.cpp src/components.h src/gameLoopConstants.h src/gameloop.h
	mkdir -p bin && g++ -std=c++11 src/main.cpp -lglut -lGLU -lGL -o bin/CaramboleStraight
