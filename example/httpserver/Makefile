

CFLAGS 		+= -g
CPPFLAGS 	+= -I./ -I../../liblwf -g -O0
LDFLAGS		+= -lpthread

CC 			= g++
LD 			= g++
AR 			= ar

LIBS		= ../../liblwf/liblwf.a
OBJS		= httpserver.o test_httpserver.o
EXEC		= test_httpserver

.PHONY: all clean $(test_appl)

test_appl: test_httpserver

all: $(test_appl)
    
test_httpserver: $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(EXEC)
