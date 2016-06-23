
CFLAGS 		+= -g
CPPFLAGS 	+= -I./
LDFLAGS		+= -lpthread

CC 			= g++
LD 			= g++
AR 			= ar

LIBS_OBJS	= logging.o eventloop.o timer.o selector.o eventhandler.o sockaddr.o \
			acceptor.o tcpconnection.o threads.o connector.o event.o event_dispatcher.o
LIBS		= liblwf.a

.PHONY: all clean

all: $(LIBS)

liblwf.a: $(LIBS_OBJS)
	$(AR) -rv $@ $^ 
	
%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(EXEC) $(LIBS)
