/* xa65 - 65xx/65816 cross-assembler and utility suite
 *
 * Copyright (C) 1989-1997 Andre Fachat (a.fachat@physik.tu-chemnitz.de)
 * Maintained by Cameron Kaiser
 *
 * PETSCII conversion module
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include <stdio.h>
#include <string.h>

#include "xacharset.h"

static signed char (*convert_func)(signed char);

static signed char convert_char_ascii(signed char c) {
	return c;
}

/* 
 * PETSCII conversion roughly follows the PETSCII to UNICODE 
 * mapping at http://www.df.lth.se/~triad/krad/recode/petscii_c64en_lc.txt
 * linked from wikipedia http://en.wikipedia.org/wiki/PETSCII
 */
static signed char convert_char_petscii(signed char c) {
	if (c >= 0x41 && c < 0x5b) {
		return c + 0x20;
	}
	if (c >= 0x61 && c < 0x7b) {
		return c - 0x20;
	}
	if (c == 0x7f) {
		return 0x14;
	}
	return c;
}

typedef struct { 
	char *name;
	signed char (*func)(signed char);
} charset;

static charset charsets[] = {
	{ "ASCII", convert_char_ascii },
	{ "PETSCII", convert_char_petscii },
	{ NULL, NULL }
};

int set_charset(char *charset_name) {
	int i = 0;
	while (charsets[i].name != NULL) {
		if (strcmp(charsets[i].name, charset_name) == 0) {
			convert_func = charsets[i].func;
			return 0;
		}
		i++;
	}
	return -1;
}

signed char convert_char(signed char c) {
	return convert_func(c);
}


