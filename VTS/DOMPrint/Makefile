
OBJS = DOMPrintErrorHandler.o DOMPrintFilter.o DOMTreeErrorReporter.o DOMPrint.o
CC = g++ -arch x86_64
CFLAGS = -g
LFLAGS = -I./../include/xerces/include -L./../include/xerces/lib -lxerces-c

all: testParser

testParser: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS)

DOMPrintErrorHandler.o: DOMPrintErrorHandler.hpp DOMPrintErrorHandler.cpp
	$(CC) $(CFLAGS) -c DOMPrintErrorHandler.cpp $(LFLAGS)	

DOMPrintFilter.o: DOMPrintFilter.hpp DOMPrintFilter.cpp
	$(CC) $(CFLAGS) -c DOMPrintFilter.cpp $(LFLAGS)

DOMTreeErrorReporter.o: DOMTreeErrorReporter.hpp DOMTreeErrorReporter.cpp
	$(CC) $(CFLAGS) -c DOMTreeErrorReporter.cpp $(LFLAGS)

DOMPrint.o: DOMPrintErrorHandler.hpp DOMPrintFilter.hpp DOMTreeErrorReporter.hpp DOMPrint.cpp
	$(CC) $(CFLAGS) -c DOMPrint.cpp $(LFLAGS)
	

clean: 
	rm *.o testParser