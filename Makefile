TuringMachine: TM.o
		g++ TM.o -o run

TM.o: TuringMachine.cpp TuringMachine.h
		g++ -c TuringMachine.cpp -o TM.o

.PHONY : clean
clean :
		rm *.o *.gch *.out $(objects)
