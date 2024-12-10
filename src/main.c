/* Main entrypoint for Brogw.
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>

#define MAX_RULES 50
#define MAX_COMMAND_LENGTH 256
#define CONFIG_PATH_LENGTH 256

// Rule structure
typedef struct {
  char pattern[128];
  char command[MAX_COMMAND_LENGTH];
} Rule;

Rule rules[MAX_RULES];
int rule_count = 0;

const char *defaultBrowser = "firefox";

/* Utility to trim whitespace */
void trim(char *str) {
  char *end;

  /* Trim leading space */
  while (isspace((unsigned char)*str)) str++;

  if (*str == 0) {
    return;
  }

  /* Trim trailing space */
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end)) end--;

  *(end + 1) = '\0';
}

/* Load configuration from file with debug messages */
int loadConfig(const char *configPath) {
  FILE *file = fopen(configPath, "r");
  if (!file) {
    return -1;
  }

  char line[512];
  while (fgets(line, sizeof(line), file)) {
    if (line[0] == '#' || strlen(line) < 3) {
      continue; /* Skip comments and empty lines */
    }

    char *pattern = strtok(line, "=");
    char *command = strtok(NULL, "\n");

    if (pattern && command) {
      trim(pattern);
      trim(command);

      if (rule_count < MAX_RULES) {
        snprintf(rules[rule_count].pattern, sizeof(rules[rule_count].pattern), "%s", pattern);
        snprintf(rules[rule_count].command, sizeof(rules[rule_count].command), "%s", command);
        rule_count++;
      }
    }
  }

  fclose(file);
  return rule_count > 0 ? 0 : -1;
}

/* Find browser command for a URL with debug messages */
const char *findBrowserCommand(const char *url) {
  static char command[MAX_COMMAND_LENGTH];
  regex_t regex;
  int reti;

  for (int i = 0; i < rule_count; i++) {
    reti = regcomp(&regex, rules[i].pattern, REG_EXTENDED);
    if (reti) {
      fprintf(stderr, "Could not compile regex\n");
      exit(1);
    }

    reti = regexec(&regex, url, 0, NULL, 0);
    regfree(&regex);

    if (!reti) {
      snprintf(command, MAX_COMMAND_LENGTH, "%s", rules[i].command);
      return command;
    }
  }

  return defaultBrowser;
}

/* Resolve configuration file */
void resolveConfigPath(char *configPath) {
  char *cwd = getenv("PWD");
  char *xdgConfig = getenv("XDG_CONFIG_HOME");

  if (cwd) {
    snprintf(configPath, CONFIG_PATH_LENGTH, "%s/config.conf", cwd);
    if (access(configPath, R_OK) == 0) {
      return;
    }
  }

  if (xdgConfig) {
    snprintf(configPath, CONFIG_PATH_LENGTH, "%s/brogw/config.conf", xdgConfig);
    if (access(configPath, R_OK) == 0) {
      return;
    }
  } else {
    snprintf(configPath, CONFIG_PATH_LENGTH, "%s/.config/brogw/config.conf", getenv("HOME"));
    if (access(configPath, R_OK) == 0) {
      return;
    }
  }

  snprintf(configPath, CONFIG_PATH_LENGTH, "/etc/brogw/config.conf");
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: brogw <URL>\n");
    return 1;
  }

  const char *url = argv[1];

  if (strncmp(url, "http://", 7) != 0 && strncmp(url, "https://", 8) != 0) {
    fprintf(stderr, "Invalid URL. Must start with http:// or https://\n");
    return 1;
  }

  char configPath[CONFIG_PATH_LENGTH];
  resolveConfigPath(configPath);

  if (loadConfig(configPath) != 0) {
    fprintf(stderr, "Error loading config\n");
    return 1;
  }

  const char *browserCommand = findBrowserCommand(url);

  char fullCommand[MAX_COMMAND_LENGTH + 256];
  snprintf(fullCommand, sizeof(fullCommand), "%s '%s'", browserCommand, url);

  int ret = system(fullCommand);
  if (ret == -1) {
    perror("Error executing command");
    return 1;
  }

  return 0;
}
