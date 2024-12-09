#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_RULES 10
#define MAX_COMMAND_LENGTH 256

// Rule structure
typedef struct {
    char pattern[128];
    char command[MAX_COMMAND_LENGTH];
} Rule;

// Define rules
Rule rules[MAX_RULES] = {
    {"https://jira\\.example\\.com", "google-chrome --profile-directory='Default'"},
    {"https://.*\\.youtube\\.com", "firefox"}
};

const char *defaultBrowser = "firefox";

// Find browser command for a URL
const char *findBrowserCommand(const char *url) {
    static char command[MAX_COMMAND_LENGTH];
    regex_t regex;
    int reti;

    for (int i = 0; i < MAX_RULES && strlen(rules[i].pattern) > 0; i++) {
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: browser-router <URL>\n");
        return 1;
    }

    const char *url = argv[1];

    if (strncmp(url, "http://", 7) != 0 && strncmp(url, "https://", 8) != 0) {
        fprintf(stderr, "Invalid URL. Must start with http:// or https://\n");
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
