
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


extern int pp_comand(char *t);
extern int pp_init(void);
extern int pp_open(char *name);
extern int pp_define(char *name);
extern void pp_close(void);
extern void pp_end(void);
extern int pgetline(char *t);
extern Datei* pp_getidat(void);

extern int ga_pp(void);
extern int gm_pp(void);
extern long gm_ppm(void);
extern long ga_ppm(void);

extern Datei *filep;
extern char s[MAXLINE];

