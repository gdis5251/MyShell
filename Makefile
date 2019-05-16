Main:Main.cpp
	g++ -o $@ $^ utilities.hpp -std=c++11

.PHONY:clean
clean:
	rm Main
