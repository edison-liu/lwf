

CFLAGS 		+= -g
CPPFLAGS 	+= -I./ -I../../liblwf -I../libopenflow -g -O0
LDFLAGS		+= -lpthread

CC 			= g++
LD 			= g++
AR 			= ar

LIBS		= ../../liblwf/liblwf.a ../libopenflow/libopenflow.a
OBJS		= of_agent.o agent.o
EXEC		= agent

.PHONY: all clean agent

all: agent
    
agent: $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(EXEC)
