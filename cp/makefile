src=$(wildcard ./src/*.cpp)
main=$(wildcard ./*.cpp)
obj=$(patsubst ./%.o, ./%.cpp, $(src) $(main)) 
include=./include
CC=g++
target=main

$(target):$(obj)
	$(CC) $^ -I $(include) -o $(target)

%.o:%.cpp
	$(CC) -c $< -o $@