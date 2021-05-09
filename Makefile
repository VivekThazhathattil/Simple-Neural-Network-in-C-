build:	./src/*.cpp
	g++ -g -std=c++17 -c ./src/*.cpp
	mkdir -p obj && mv ./*.o ./obj
	g++ -g ./obj/*.o -o main
	chmod 755 ./main
	./main

clean: 
	rm -rf ./obj
	rm -rf ./saveData/*.dat
