
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


extern void errout(int er);
extern void logout(char *s);

extern int ncmos,cmosfl,w65816,n65816;
extern int masm, nolink;
extern int noglob;
extern int showblk;
extern int relmode;
extern int crossref;

extern int tlen, tbase;
extern int blen, bbase;
extern int dlen, dbase;
extern int zlen, zbase;
extern int romable, romadr;

extern int memode,xmode;
extern int segment;
extern int pc[SEG_MAX];

extern int h_length(void);

extern void set_align(int align_value);

