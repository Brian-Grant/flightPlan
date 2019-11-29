
all:	FDS


FDS:	driver.o FDS.o Passenger.o Flight.o Time.o City.o
	g++ -O0 -g -Wall -std=c++11 driver.o FDS.o Flight.o City.o Passenger.o Time.o -o FDS

driver.o:	driver.cpp
	g++ -O0 -g -Wall -std=c++11 -c driver.cpp

FDS.o:	FDS.cpp
	g++ -O0 -g -Wall -std=c++11 -c FDS.cpp

Flight.o:	Flight.cpp
	g++ -O0 -g -Wall -std=c++11 -c Flight.cpp

Passenger.o:	Passenger.cpp
	g++ -O0 -g -Wall -std=c++11 -c Passenger.cpp


Time.o:	Time.cpp
	g++ -O0 -g -Wall -std=c++11 -c Time.cpp

City.o: City.cpp
	g++ -O0 -g -Wall -std=c++11 -c City.cpp

clean:
	rm -f *.o FDS

testany:	FDS
	(./FDS schedule.txt any) < in.txt

testany2:	FDS
	(./FDS flight_file.txt any) < in2.txt

testearl:	FDS
	(./FDS schedule.txt earliest) < in.txt

testearl2:	FDS
	(./FDS flight_file.txt earliest) < in2.txt


test3:	FDS
	(./FDS schedule.txt cheapest) < in.txt

gdb:	FDS
	gdb ./FDS 
	

valg:	FDS
	(valgrind ./FDS schedule.txt any) < in.txt

valg2:	FDS
	(valgrind ./FDS flight_file.txt any) < in2.txt

valg3:	FDS
	(valgrind ./FDS schedule.txt earliest) < in.txt

valg4:	FDS
	(valgrind ./FDS flight_file.txt earliest) < in2.txt

valgr: FDS
	(valgrind -v ./FDS schedule.txt any) < in.txt
	(valgrind --leak-check=full ./FDS schedule.txt any) < in.txt

