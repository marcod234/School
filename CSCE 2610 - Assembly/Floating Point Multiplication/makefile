CC=g++

MULT: main.o func.o
	$(CC) -o MULT main.o func.o
  
main.o: main.cpp head.h
	$(CC) -c main.cpp
	
func.o: func.cpp head.h
	$(CC) -c func.cpp
  
clean:
	rm MULT main.o func.o
  
run: MULT
	./MULT