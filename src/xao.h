
/*
    xa65 - 6502 cross assembler and utility suite
    Copyright (C) 1989-1997 Andr� Fachat (a.fachat@physik.tu-chemnitz.de)

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


/* sets file option after pass 1 */
extern void set_fopt(int l, signed char *buf, int reallen);

/* writes file options to a file */
extern void o_write(FILE *fp);

/* return overall length of header options */
extern size_t o_length(void);

