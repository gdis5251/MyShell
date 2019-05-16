Main:Main.cpp
	g++ -o $@ $^ utilities.hpp builtincommand.hpp -std=c++11

.PHONY:clean
clean:
	rm Main
