OBJS = calmat.o comparim.o creation-graphe.o main_new.o	tas.o creation-graphe2.o myalloc.o \
testfig.o creation-graphe3.o reduboul.o	finitcyl3.o regboules.o simulation.o testfig5.o \
testfig6.o geometrie.o image2opendx.o init-cyl.o routines.o selectboules.o squelette.o testfig7.o simulation-new.o
#  -ffast-math             Allow aggressive, lossy floating-point optimizations ???
CFLAGS = -O3 
simulation:$(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm *.o




