program : pfspinstance.h flowshop.h flowshop.cpp pfspinstance.cpp main.cpp

	g++ -O3 -std=c++11 -c flowshop.cpp -o flowshop.o
	g++ -O3 -std=c++11 -c pfspinstance.cpp -o pfspinstance.o
	g++ -O3 -std=c++11 -c main.cpp -o main.o

	g++ -O3 -std=c++11 flowshop.o pfspinstance.o main.o -o flowshopRUN

clean:
	rm /*.o flowshopRUN
