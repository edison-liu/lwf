
CFLAGS 		+= -g
CPPFLAGS 	+= -I../liblwf -g -O0
LDFLAGS		+= -lpthread

CC 			= g++
LD 			= g++
AR 			= ar

LIBS		= ../liblwf/liblwf.a
EXEC		= test_threads test_tcpserver test_acceptor test_logging test_eventloop test_eventhandler

.PHONY: all clean $(test_appl)

test_appl: test_threads # test_tcpserver #test_acceptor #test_eventhandler #test_eventloop #test_logging

all: $(test_appl)
    
test_logging: test_logging.o $(LIBS)
	$(CC) $(LDFLAGS) $^ -o $@

test_eventloop: test_eventloop.o $(LIBS)
	$(CC) $(LDFLAGS) $^ -o $@
	
test_eventhandler: test_eventhandler.o $(LIBS)
	$(CC) $(LDFLAGS) $^ -o $@	

test_acceptor: test_acceptor.o $(LIBS)
	$(CC) $(LDFLAGS) $^ -o $@

test_tcpserver: test_tcpserver.o $(LIBS)
	$(CC) $(LDFLAGS) $^ -o $@

test_threads: test_threads.o $(LIBS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(EXEC)
