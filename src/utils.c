/* utils.h - Common utility functions.
   Copyright (C) 2024 Free Software Foundation, Inc.

This file is part of Brogw.

Brogw is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Brogw is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License
along with Brigw.  If not, see <https://www.gnu.org/licenses/>.  */


#include <ctype.h>
#include <string.h>

#include "utils.h"


/*
 * Trims leading and trailing whitespace in-place.
 *
 * Note: This function modifies the input string buffer directly
 * and may shift the starting position if leading spaces are removed.
 * If preserving the original pointer offset is required, consider
 * copying the trimmed substring back to the original start.
 */
void trim(char *str) {
  char *end;

  /* Trim leading space */
  while (isspace((unsigned char)*str)) str++;

  if (*str == 0) return;

  /* Trim trailing space */
  end = str + strlen(str) - 1;

  while (end > str && isspace((unsigned char)*end)) end--;

  *(end + 1) = '\0';
}
