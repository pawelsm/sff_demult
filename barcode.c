/* L I C E N S E *************************************************************/

/*
 	Copyright (C) 2014 Pawel Smialowski

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, see <http://www.gnu.org/licenses/>
*/

/* I N C L U D E S ***********************************************************/
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

/* F U N C T I O N S *********************************************************/
int
strncmp_with_mismatch (const char *s1,
					const char *s2,
					register size_t n,
					register size_t mismatch) {
	register unsigned char u1, u2;
	int cnt=0;
	while (n-- > 0) {
		u1 = (unsigned char) *s1++;
		u2 = (unsigned char) *s2++;
		if (u1 != u2) {
			cnt++;
			if (cnt > mismatch) return u1 - u2;
		}
		if (u1 == '\0') return 0;
	}
	return 0;
}
