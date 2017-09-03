#Makefile

CXX = g++
CXXFLAGS =
#-ansi -pedantic -g

all: Server Cliente


Server: Server.cpp Socket.cpp Socket.h Semaforos.o
	$(CXX) $(CXXFLAGS) -o Server Server.cpp Socket.cpp -lpthread

Cliente: Cliente.cpp Socket.cpp Socket.h
	$(CXX) $(CXXFLAGS) -o Cliente Cliente.cpp Socket.cpp

Thread.o: threads.h threads.cpp
	$(CXX) $(CXXFLAGS) -c threads.h threads.cpp -lpthread

Semaforos.o: semaforos.h semaforos.c
	$(CXX) $(CXXFLAGS) -c semaforos.h semaforos.c

Mutex.o: Mutex.h Mutex.cpp
	$(CXX) $(CXXFLAGS) -c Mutex.h Mutex.cpp

clean:
	$(RM) Server Cliente *.o *.gch
