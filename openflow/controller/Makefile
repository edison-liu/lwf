

CFLAGS 		+= -g
CPPFLAGS 	+= -I./ -I../../liblwf -I../libopenflow -g -O0
LDFLAGS		+= -lpthread

CC 			= g++
LD 			= g++
AR 			= ar

LIBS		= ../../liblwf/liblwf.a ../libopenflow/libopenflow.a
OBJS		= of_controller.o controller.o
EXEC		= controller

.PHONY: all clean controller

all: controller
    
controller: $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(EXEC)
