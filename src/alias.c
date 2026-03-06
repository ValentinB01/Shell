#include "../include/shell.h"
#include <ctype.h>

#define MAX_ALIASES 64

typedef struct {
    char *name;
    char *value;
} Alias;

static Alias alias_table[MAX_ALIASES];
static int   alias_count = 0;

int alias_set(const char *name, const char *value) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias_table[i].name, name) == 0) {
            free(alias_table[i].value);
            alias_table[i].value = strdup(value);
            return 0;
        }
    }
    if (alias_count >= MAX_ALIASES) {
        fprintf(stderr, "alias: limita de %d alias-uri atinsa\n", MAX_ALIASES);
        return -1;
    }
    alias_table[alias_count].name  = strdup(name);
    alias_table[alias_count].value = strdup(value);
    alias_count++;
    return 0;
}

int alias_unset(const char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias_table[i].name, name) == 0) {
            free(alias_table[i].name);
            free(alias_table[i].value);
            alias_table[i] = alias_table[--alias_count];
            return 0;
        }
    }
    fprintf(stderr, "unalias: '%s' nu este definit\n", name);
    return -1;
}

void alias_print(const char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (name == NULL || strcmp(alias_table[i].name, name) == 0) {
            printf("alias %s='%s'\n", alias_table[i].name, alias_table[i].value);
        }
    }
}


void alias_expand(const char *line, char *out_buf, size_t out_size) {
    while (*line && isspace((unsigned char)*line)) line++;

    const char *end = line;
    while (*end && !isspace((unsigned char)*end)) end++;

    size_t name_len = (size_t)(end - line);

    for (int i = 0; i < alias_count; i++) {
        if (strlen(alias_table[i].name) == name_len &&
            strncmp(alias_table[i].name, line, name_len) == 0)
        {
            snprintf(out_buf, out_size, "%s%s",
                     alias_table[i].value, end);
            return;
        }
    }

    strncpy(out_buf, line, out_size - 1);
    out_buf[out_size - 1] = '\0';
}
