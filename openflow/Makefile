
LIBSRC		    = libopenflow
AGENTSRC	    = agent
CONTROLLERSRC	= controller

.PHONY: libopenflow agent controller

all: libopenflow agent controller
    
libopenflow:
	make -C $(LIBSRC)

agent:
	make -C $(AGENTSRC)

controller:
	make -C $(CONTROLLERSRC)

clean:
	make -C $(LIBSRC) $@
	make -C $(AGENTSRC) $@
	make -C $(CONTROLLERSRC) $@
