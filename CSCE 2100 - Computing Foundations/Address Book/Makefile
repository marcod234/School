prog4: main.o func.o ftree.o
	g++ -o prog4 main.o func.o ftree.o

main.o: D_00_P4_Main.cpp D_00_P4_Header.h
	g++ -o main.o -c D_00_P4_Main.cpp

func.o: D_00_P4_Func.cpp D_00_P4_Header.h
	g++ -o func.o -c D_00_P4_Func.cpp
	
ftree.o: D_00_P4_Ftree.cpp D_00_P4_Header.h
	g++ -o ftree.o -c D_00_P4_Ftree.cpp

clean:
	rm prog4 main.o func.o ftree.o

run: prog4
	./prog4
