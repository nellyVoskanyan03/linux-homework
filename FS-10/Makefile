all: copy

hacker-rm: copy.o
	g++ copy.o -o copy

hacker-rm.o: copy.cpp
	g++ -c copy.cpp

clean:
	rm *.o copy
