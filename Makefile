
# Unix gcc or DOS go32 cross-compiling gcc
#
CC=gcc
LD=gcc
CFLAGS=-W -Wall -pedantic -ansi -g
# CC=gcc-go32
# LD=gcc-go32
# CFLAGS=-W -Wall -pedantic 

# Other cc
#CC=cc
#CFLAGS=
#LD=ld

INSTALLDIR=/usr/local

all: xa uncpk

xa:
	(cd src; LD=${LD} CC="${CC} ${CFLAGS}" ${MAKE}; )

load:	
	(cd loader; CC="${CC} ${CFLAGS}" ${MAKE}; )

uncpk:
	(cd misc; CC="${CC} ${CFLAGS}" ${MAKE}; )

dos: clean
	(cd src; LD=gcc-go32 CC="gcc-go32 -W -Wall -pedantic" ${MAKE}; )
	(cd misc; CC="gcc-go32 -W -Wall -pedantic" ${MAKE}; )
	rm -f xa file65 ld65 uncpk printcbm reloc65 mkrom.sh src/*.o

clean:
	(cd src; ${MAKE} clean )
	(cd loader; ${MAKE} clean )
	(cd misc; ${MAKE} mrproper )
	rm -f xa *.exe

install: xa uncpk
	for i in xa reloc65 ld65 file65 printcbm uncpk; 	\
	  do cp $$i ${INSTALLDIR}/bin; 				\
	done

