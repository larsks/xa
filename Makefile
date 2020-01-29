
CC=gcc
CFLAGS=-W -Wall -pedantic -ansi
LD=gcc

#CC=cc
#CFLAGS=
#LD=cc

INSTALLDIR=/usr/local

all: xa uncpk load

xa:
	(cd src; LD=${LD} CC="${CC} ${CFLAGS}" ${MAKE}; )

load:	
	(cd loader; CC="${CC} ${CFLAGS}" ${MAKE}; )

uncpk:
	(cd misc; CC="${CC} ${CFLAGS}" ${MAKE}; )

clean:
	(cd src; ${MAKE} clean )
	(cd loader; ${MAKE} clean )
	(cd misc; ${MAKE} mrproper )
	rm -f xa

install: xa uncpk
	for i in xa reloc65 ld65 file65 printcbm uncpk; 	\
	  do cp $$i ${INSTALLDIR}/bin; 				\
	done

