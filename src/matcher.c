/* matcher.c
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

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matcher.h"

const char *matcher_find_command(const char *url, const Rule *rules,
                                 size_t cnt, const char *default_browser) {
  static char command[MAX_COMMAND_LENGTH];
  regex_t regex;
  int reti;

  for (size_t i = 0; i < cnt; i++) {
    reti = regcomp(&regex, rules[i].pattern, REG_EXTENDED);
    if (reti != 0) {
      /* If regex compilation fails, skip this rule */
      continue;
    }

    reti = regexec(&regex, url, 0, NULL, 0);
    regfree(&regex);

    if (!reti) {
      snprintf(command, sizeof(command), "%s", rules[i].command);
      return command;
    }
  }

  return default_browser;
}
