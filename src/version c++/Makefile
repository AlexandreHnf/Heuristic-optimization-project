program : pfspinstance.h flowshop.h flowshop.cpp pfspinstance.cpp main.cpp

	g++ -O3 -c flowshop.cpp -o flowshop.o
	g++ -O3 -c pfspinstance.cpp -o pfspinstance.o
	g++ -O3 -c main.cpp -o main.o

	g++ -O3 flowshop.o pfspinstance.o main.o -o flowshopRUN

clean:
	rm /*.o flowshopRUN
