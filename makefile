# *  Define the name of the makefile.                                        *

MAKNAM = makefile

# *  Define the directories in which to search for library files.            *

LIBDRS =

# *  Define the directories in which to search for include files.            *

INCDRS =

# *  Define the library files.                                               *

LIBFLS =

# *  Define the source files.                                                *

SRCFLS = vm.c\
         collector.c\
	     heap.c\
         bistree.c\
		 list.c\
	     mutator.c\
		 lib.c\

# *  Define the object files.                                                *

VM_OBJFLS = vm.o\
         collector.o\
         heap.o\
         bistree.o\
	     list.o\
		 lib.o\

MUTATOR_OBJFLS = mutator.o\
         collector.o\
         heap.o\
         bistree.o\
         list.o\

# *  Define the executable.                                                  *

MUTATOR_EXE    = mutator

VM_EXE         = vm

# *  Define the compile and link options.                                    *

CC     = gcc
LL     = gcc
CFLAGS = -Wall
LFLAGS =

# *  Define the lib name to export ot OCaml                                  *
LIB_NAME = vm.so

# *  Define the rules.                                                       *

$(VM_EXE): $(VM_OBJFLS)
	$(LL) $(LFLAGS) $(VM_OBJFLS) -o $@ $(LIBDRS) $(LIBFLS)

$(MUTATOR_EXE): $(MUTATOR_OBJFLS)
	$(LL) $(LFLAGS) $(MUTATOR_OBJFLS) -o $@ $(LIBDRS) $(LIBFLS)

.c.o:
	$(CC) $(CFLAGS) $(INCDRS) -c $<

all:
	make -f $(MAKNAM) clean
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_MS" $(VM_EXE)
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_MS" $(MUTATOR_EXE)

ms:
	make -f $(MAKNAM) clean
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_MS" $(VM_EXE)
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_MS" $(MUTATOR_EXE)

mc:
	make -f $(MAKNAM) clean
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_MC" $(VM_EXE)
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_MC" $(MUTATOR_EXE)

cc:
	make -f $(MAKNAM) clean
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_CC" $(VM_EXE)
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_CC" $(MUTATOR_EXE)

lib-ms:
	make -f $(MAKNAM) clean
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_MS" $(VM_EXE)
	$(LL) -shared -o $(LIB_NAME) $(VM_OBJFLS)

lib-mc:
	make -f $(MAKNAM) clean
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_MC" $(VM_EXE)
	$(LL) -shared -o $(LIB_NAME) $(VM_OBJFLS)

lib-cc:
	make -f $(MAKNAM) clean
	make -f $(MAKNAM) CFLAGS="$(CFLAGS) -D_CC" $(VM_EXE)
	$(LL) -shared -o $(LIB_NAME) $(VM_OBJFLS)


clean:
	-rm $(VM_EXE)
	-rm $(MUTATOR_EXE)
	-rm $(VM_OBJFLS)
	-rm $(MUTATOR_OBJFLS)
	-rm $(LIB_NAME)

# DO NOT DELETE THIS LINE -- make depend depends on it.

mutator.o: bool.h globals.h list.h bistree.h bistree.c
heap.o: heap.h globals.h heap.c
bistree.o: bool.h bistree.h bistree.c
list.o: bool.h list.h list.c
