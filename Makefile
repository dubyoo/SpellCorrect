./bin/miniSearchServer:./src/*.cpp
	g++ -o $@ $^ -I./include -lpthread -std=c++11
