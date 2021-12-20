src=$(wildcard src/*.cpp)

main:
	g++ -std=c++11 -g \
	$(src) \
	main.cpp \
	-o main