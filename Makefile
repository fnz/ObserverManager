CC=g++
CFLAGS=-c -Wall -std=c++11 -stdlib=libc++ -IObserverManager
LDFLAGS=-stdlib=libc++
SOURCES=main.cpp ObserverManager/BaseObserverProtocol.cpp ObserverManager/ObserverManager.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=RunTests

all: $(SOURCES) $(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(RM) $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@