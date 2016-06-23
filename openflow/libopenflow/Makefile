

CFLAGS 		+= -g
CPPFLAGS 	+= -I./ -I../../liblwf -g -O0
LDFLAGS		+= -lpthread

CC 			= g++
LD 			= g++
AR 			= ar

LIBS		= 
OBJS		= openflow.o of_connection.o openflow_packet.o
EXEC		= 

.PHONY: all clean $(libopenflow)

libopenflow: libopenflow.a

all: $(libopenflow)
    
libopenflow.a: $(OBJS)
	$(AR) -rv $@ $^ 

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(EXEC)
