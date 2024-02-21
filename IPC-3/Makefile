first: first.o shared_array.o
	g++ first.o shared_array.o -o first

first.o: first.cpp
	g++ first.cpp -c -o first.o 
	
second: second.o shared_array.o
	g++ second.o shared_array.o -o second

second.o: second.cpp
	g++ second.cpp -c -o second.o 

shared_array.o: shared_array.cpp
	g++ shared_array.cpp -c -o shared_array.o 

clean:
	rm *.o first second