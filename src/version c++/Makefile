program : pfspinstance.h flowshop.h flowshop.cpp pfspinstance.cpp main.cpp

	g++ -O3 -std=c++11 -pthread -c flowshop.cpp -o flowshop.o
	g++ -O3 -std=c++11 -pthread -c pfspinstance.cpp -o pfspinstance.o
	g++ -O3 -std=c++11 -pthread -c main.cpp -o main.o

	g++ -O3 -std=c++11 -pthread flowshop.o pfspinstance.o main.o -o flowshopRUN

clean:
	rm /*.o flowshopRUN
