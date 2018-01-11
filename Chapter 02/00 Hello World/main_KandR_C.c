#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

static bool readline(char buffer[]);
static int index_of(char str[], char needle[]);

int main(void)
{
    char instr[MAX_LINE_LENGTH], needle[MAX_LINE_LENGTH];
    printf("This echos input that contains a user-defined needle\n"
           "Type :exit to terminate.\nEnter search string:\n");
    if (readline(needle)) {
        printf("Enter strings\n");
        register bool matches;
        while (readline(instr)) {
            matches = (index_of(instr, needle) >= 0);
            printf("%s", matches ? instr : "");
            printf("%s", matches ? "\n" : "");
        }
    }
    return EXIT_SUCCESS;
}

static bool readline(char buffer[])
{
    printf("$ ");
    fgets(buffer, MAX_LINE_LENGTH, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    return (strcmp(buffer, ":exit\n") != 0);
}

static int index_of(char str[], char needle[])
{
    register char str_c, str_scan_c, needle_c;
    register int match_idx;
    register bool match_found;

    for (match_idx = 0, match_found = false;
         str_c = str[match_idx], str_c != '\0' && !match_found;
         match_idx++) {
        for (unsigned j = 0; needle_c = needle[j], str_scan_c = str[match_idx + j],
             str_c == needle[0] && needle_c != '\0' && str_scan_c != '\0' && str_scan_c == needle_c;
             j++) {
            match_found |= (needle[j + 1] == '\0');
        }
    }
    return match_found ? match_idx : -1;
}
