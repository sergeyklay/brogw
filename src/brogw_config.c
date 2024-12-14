/* brogw_config.c
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
along with Brogw.  If not, see <https://www.gnu.org/licenses/>.  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "utils.h"
#include "brogw_config.h"

static int read_config_file(const char *config_path, Rule **rules, size_t *cnt);
static void resolve_config_path(char *config_path, size_t len);

int config_load_rules(Rule **rules, size_t *cnt) {
  char config_path[CONFIG_PATH_LENGTH];
  resolve_config_path(config_path, sizeof(config_path));

  Rule *rules_int = NULL;
  size_t count = 0;

  if (read_config_file(config_path, &rules_int, &count) != 0) {
    return -1;
  }

  *rules = rules_int;
  *cnt = count;
  return 0;
}

void config_free_rules(Rule *rules, size_t cnt) {
  if (!rules) return;

  for (size_t i = 0; i < cnt; i++) {
    free(rules[i].pattern);
    free(rules[i].command);
  }

  free(rules);
}

static void resolve_config_path(char *config_path, size_t len) {
  const char *cwd = getenv("PWD");
  if (cwd) {
    snprintf(config_path, len, "%s/config.conf", cwd);
    if (access(config_path, R_OK) == 0) return;
  }

  const char *xdg_config = getenv("XDG_CONFIG_HOME");
  if (xdg_config) {
    snprintf(config_path, len, "%s/brogw/config.conf", xdg_config);
    if (access(config_path, R_OK) == 0) return;
  } else {
    const char *home = getenv("HOME");
    if (home) {
      snprintf(config_path, len, "%s/.config/brogw/config.conf", home);
      if (access(config_path, R_OK) == 0) return;
    }
  }

  snprintf(config_path, len, "/etc/brogw/config.conf");
}

static int read_config_file(const char *path, Rule **rules, size_t *cnt) {
  FILE *f = fopen(path, "r");
  if (!f) return -1;

  char line[512];
  size_t capacity = 10;
  Rule *list = malloc(sizeof(Rule)*capacity);
  if (!list) {
    fclose(f);
    return -1;
  }

  size_t count = 0;
  while (fgets(line, sizeof(line), f)) {
    if (line[0] == '#' || strlen(line) < 3) {
      continue; /* Skip comments and empty lines */
    }

    char *sep = strchr(line, '=');
    if (!sep) continue;

    *sep = '\0';
    char *pattern = line;
    char *command = sep+1;

    trim(pattern);
    trim(command);

    if (strlen(pattern) == 0 || strlen(command) == 0)
      continue;

    if (count == capacity) {
      capacity *= 2;
      Rule *new_list = realloc(list, sizeof(Rule)*capacity);
      if (!new_list) {
        free(list);
        fclose(f);
        return -1;
      }
      list = new_list;
    }

    list[count].pattern = strdup(pattern);
    list[count].command = strdup(command);
    count++;
  }

  fclose(f);

  *rules = list;
  *cnt = count;
  return 0;
}
