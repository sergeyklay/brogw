/* main.c - Main entrypoint for Brogw.
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

#include "brogw_config.h"
#include "utils.h"
#include "matcher.h"
#include "executor.h"


int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: brogw <URL>\n");
    return 1;
  }

  if (argv[1][0] == '\0') {
    fprintf(stderr, "Error: Empty URL or file path provided\n");
    return 1;
  }

  const char *url = argv[1];
  Rule *rules = NULL;
  size_t rule_count = 0;

  if (config_load_rules(&rules, &rule_count) != 0) {
    fprintf(stderr, "Error loading configuration\n");
    return 1;
  }

  const char *default_browser = "firefox";
  const char *command = matcher_find_command(url, rules, rule_count, default_browser);

  int ret = executor_run_command(command, url);
  if (ret != 0) {
    fprintf(stderr, "Failed to execute command: %s\n", command);
  }

  // In the future, we will integrate matcher and executor here.
  config_free_rules(rules, rule_count);
  return ret;
}
