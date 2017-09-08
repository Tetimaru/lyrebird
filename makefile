###################################################################
#
# Makefile
#
###################################################################
.SUFFIXES: .h .o .c

CCMAIN = main.c
CC = gcc
CCOPTS = -g
LIBS = -lm
OBJS = main.o decrypt.o memwatch.o
CCEXEC = lyrebird

all:	$(CCEXEC)

$(CCEXEC):	$(OBJS) makefile 
	@echo Linking $@ . . .
	$(CC) $(CCOPTS) $(OBJS) -o $@ $(LIBS) 

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) -DMEMWATCH -DMW_STDIO -c $(CCOPTS) $< memwatch.c

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC)
	rm -f core
