CC = g++
CFLAGS = -g -Wall -Werror -std=c++11
OBJECTS = simulator.o object.o service.o link.o

all: first second third forth fifth sixth

first: first.o $(OBJECTS)
	$(CC) $(CFLAGS) -o first first.o $(OBJECTS)

second: second.o $(OBJECTS)
	$(CC) $(CFLAGS) -o second second.o $(OBJECTS)

third: third.o $(OBJECTS)
	$(CC) $(CFLAGS) -o third third.o $(OBJECTS)

forth: forth.o $(OBJECTS)
	$(CC) $(CFLAGS) -o forth forth.o $(OBJECTS)

fifth: fifth.o $(OBJECTS)
	$(CC) $(CFLAGS) -o fifth fifth.o $(OBJECTS)

sixth: sixth.o $(OBJECTS)
	$(CC) $(CFLAGS) -o sixth sixth.o $(OBJECTS)

first.o: scenarios/first.cpp
	$(CC) $(CFLAGS) -c scenarios/first.cpp

second.o: scenarios/second.cpp
	$(CC) $(CFLAGS) -c scenarios/second.cpp

third.o: scenarios/third.cpp
	$(CC) $(CFLAGS) -c scenarios/third.cpp

forth.o: scenarios/forth.cpp
	$(CC) $(CFLAGS) -c scenarios/forth.cpp

fifth.o: scenarios/fifth.cpp
	$(CC) $(CFLAGS) -c scenarios/fifth.cpp

sixth.o: scenarios/sixth.cpp
	$(CC) $(CFLAGS) -c scenarios/sixth.cpp

simulator.o: simulator.cpp
	$(CC) $(CFLAGS) -c simulator.cpp

object.o: object.cpp
	$(CC) $(CFLAGS) -c object.cpp

service.o: service.cpp
	$(CC) $(CFLAGS) -c service.cpp

link.o: link.cpp
	$(CC) $(CFLAGS) -c link.cpp

clean:
	rm -f *.o *.exe first second third forth fifth sixth
