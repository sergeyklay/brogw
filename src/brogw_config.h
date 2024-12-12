/* src/brogw_config.h
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


#ifndef BROGW_BROGW_CONFIG_H
#define BROGW_BROGW_CONFIG_H

#include <stddef.h>

#define MAX_COMMAND_LENGTH 256
#define CONFIG_PATH_LENGTH 256

typedef struct {
  char *pattern;
  char *command;
} Rule;

/* Load configuration rules from files based on priority.
   Returns 0 on success, non-zero on failure.
   Allocates memory for rules and stores the pointer in *rules.
   Caller is responsible for calling config_free_rules(). */
int config_load_rules(Rule **out, size_t *cnt);

/* Free memory allocated for rules. */
void config_free_rules(Rule *rules, size_t cnt);

#endif /* BROGW_BROGW_CONFIG_H */
