# Unix gcc or DOS go32 cross-compiling gcc
#
CC = gcc
LD = gcc
CFLAGS = -O2 -W -Wall -pedantic -ansi
# CC = gcc-go32
# LD = gcc-go32
# CFLAGS = -W -Wall -pedantic 

# Other cc
#CC = cc
#CFLAGS =
#LD = ld

DESTDIR = /usr/local

BINDIR = $(DESTDIR)/bin
MANDIR = $(DESTDIR)/share/man/man1
DOCDIR = $(DESTDIR)/share/doc

MKDIR = mkdir -p
INSTALL = install

all: xa uncpk

xa:
	(cd src && LD=${LD} CC="${CC} ${CFLAGS}" ${MAKE})

load:	
	(cd loader && CC="${CC} ${CFLAGS}" ${MAKE})

uncpk:
	(cd misc && CC="${CC} ${CFLAGS}" ${MAKE})

dos: clean
	(cd src && LD=gcc-go32 CC="gcc-go32 -W -Wall -pedantic" ${MAKE})
	(cd misc && CC="gcc-go32 -W -Wall -pedantic" ${MAKE})
	rm -f xa file65 ldo65 uncpk printcbm reloc65 mkrom.sh src/*.o

clean:
	(cd src && ${MAKE} clean)
	(cd loader && ${MAKE} clean)
	(cd misc && ${MAKE} mrproper)
	rm -f xa *.exe

install: xa uncpk
	$(MKDIR) $(BINDIR)
	$(MKDIR) $(MANDIR)
	$(INSTALL) xa reloc65 ldo65 file65 printcbm uncpk $(BINDIR)
	$(INSTALL) man/file65.1 man/ldo65.1 man/printcbm.1 man/reloc65.1 man/uncpk.1 man/xa.1 $(MANDIR)
	#$(MKDIR) $(DOCDIR)/xa65

dist: clean
	cd .. ; tar cvf xa-2.3.0.tar xa-2.3.0 ; gzip xa-2.3.0.tar
