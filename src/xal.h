
/*
    xa65 - 6502 cross assembler and utility suite
    Copyright (C) 1989-1997 André Fachat (a.fachat@physik.tu-chemnitz.de)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#include <stdio.h> 

extern char   *lz;

extern int l_init(void);
extern int ga_lab(void);
extern int gm_lab(void);
extern long gm_labm(void);
extern long ga_labm(void);

extern int lg_set(char *);

extern int b_init(void);
extern int b_depth(void);

extern void printllist(FILE *fp);
extern int ga_blk(void);

extern int l_def(char *s, int* l, int *x, int *f);
extern int l_such(char *s, int *l, int *x, int *v, int *afl);
extern void l_set(int n, int v, int afl);
extern int l_get(int n, int *v, int *afl);
extern int l_vget(int n, int *v, char **s);
extern int ll_such(char *s, int *n);     
extern int ll_pdef(char *t);

extern int b_open(void);
extern int b_close(void);

extern int l_write(FILE *fp);

