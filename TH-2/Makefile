all: parallel_scheduler.so demo-application

parallel_scheduler.so: parallel_scheduler.cpp
	g++ -shared -fPIC -o parallel_scheduler.so parallel_scheduler.cpp -ldl -lpthread

demo-application: demo-application.o
	g++ demo-application.o -o demo-application

demo-application.o: demo-application.cpp
	g++ -c demo-application.cpp

clean:
	rm -f parallel_scheduler.so demo-application *.o
