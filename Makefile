all: complexN

complexN: main.o	complexNumber.o
	g++ -o complexN	main.o	complexNumber.o

complexNumber.0: complexNumbere.cpp	complexNumber.h
	g++ -c complexNumber.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o complexN
