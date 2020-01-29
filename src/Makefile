
OBJ = xa.o xaa.o xal.o xap.o xat.o xar.o xar2.o xao.o xau.o xam.o

#CFLAGS=-W -Wall -pedantic -ansi  #-g

LD=${CC}

all: xa

xa: ${OBJ}
	${LD}  -o ../xa ${OBJ} -lc

clean: 
	rm -f *.o

mrproper: clean
	rm -f ../xa

