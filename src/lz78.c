#define CCOMPRA_LIBRARY_BUILD

#include "../include/ccompra/lz78.h"

CCOMPRA_PUBLIC_FUNC
char* lz78_compress(const char *input) {
    size_t inLen = strlen(input);
    size_t tokenCap = 128;
    LZ78Token *tokens = malloc(tokenCap * sizeof(LZ78Token));
    if (!tokens) { perror("malloc"); exit(EXIT_FAILURE); }
    size_t tokenCount = 0;

    /* Use a simple dynamic array of dictionary strings */
    size_t dictCap = 128, dictSize = 0;
    char **dictionary = malloc(dictCap * sizeof(char*));
    if (!dictionary) { perror("malloc"); exit(EXIT_FAILURE); }
    dictionary[dictSize++] = strdup("");  // index 0: empty string

    size_t pos = 0;
    while (pos < inLen) {
        size_t currentIndex = 0;
        size_t currentLen = 0;
        char buffer[1024] = "";
        size_t curPos = pos;
        while (curPos < inLen) {
            buffer[currentLen] = input[curPos];
            buffer[currentLen+1] = '\0';
            int found = -1;
            for (size_t i = 0; i < dictSize; i++) {
                if (strcmp(dictionary[i], buffer) == 0) { found = i; break; }
            }
            if (found == -1)
                break;
            currentIndex = found;
            currentLen++;
            curPos++;
            if (currentLen >= sizeof(buffer)-1)
                break;
        }
        LZ78Token t;
        t.index = currentIndex;
        t.next = (curPos < inLen) ? input[curPos] : '\0';
        tokens[tokenCount++] = t;
        /* Add new entry to dictionary */
        char newEntry[1024];
        snprintf(newEntry, sizeof(newEntry), "%s%c", dictionary[currentIndex], t.next);
        if (dictSize >= dictCap) {
            dictCap *= 2;
            dictionary = realloc(dictionary, dictCap * sizeof(char*));
        }
        dictionary[dictSize++] = strdup(newEntry);
        pos = curPos + 1;
        if (tokenCount >= tokenCap) {
            tokenCap *= 2;
            tokens = realloc(tokens, tokenCap * sizeof(LZ78Token));
        }
    }
    /* Serialize tokens into a string */
    size_t outCap = tokenCount * 32 + 1;
    char *output = malloc(outCap);
    if (!output) { perror("malloc"); exit(EXIT_FAILURE); }
    output[0] = '\0';
    for (size_t i = 0; i < tokenCount; i++) {
        char token[32];
        snprintf(token, sizeof(token), "T,%zu,%d;", tokens[i].index, (int)tokens[i].next);
        if (strlen(output) + strlen(token) + 1 > outCap) {
            outCap *= 2;
            output = realloc(output, outCap);
        }
        strcat(output, token);
    }
    for (size_t i = 0; i < dictSize; i++)
        free(dictionary[i]);
    free(dictionary);
    free(tokens);
    return output;
}

CCOMPRA_PUBLIC_FUNC
char* lz78_decompress(const char *compressed) {
    /* Deserialize tokens first */
    size_t tokenCap = 128, tokenCount = 0;
    LZ78Token *tokens = malloc(tokenCap * sizeof(LZ78Token));
    if (!tokens) { perror("malloc"); exit(EXIT_FAILURE); }
    const char *p = compressed;
    while (*p) {
        size_t index;
        int nextInt;
        if (sscanf(p, "T,%zu,%d;", &index, &nextInt) != 2)
            break;
        if (tokenCount >= tokenCap) {
            tokenCap *= 2;
            tokens = realloc(tokens, tokenCap * sizeof(LZ78Token));
        }
        tokens[tokenCount].index = index;
        tokens[tokenCount].next = (char)nextInt;
        tokenCount++;
        const char *semicolon = strchr(p, ';');
        if (!semicolon)
            break;
        p = semicolon + 1;
    }
    /* Build dictionary and output string */
    size_t dictCap = 128, dictSize = 0;
    char **dictionary = malloc(dictCap * sizeof(char*));
    dictionary[dictSize++] = strdup("");
    size_t outCap = 256;
    char *output = malloc(outCap);
    output[0] = '\0';
    for (size_t i = 0; i < tokenCount; i++) {
        LZ78Token t = tokens[i];
        const char *entry = (t.index < dictSize) ? dictionary[t.index] : "";
        size_t entryLen = strlen(entry);
        while (strlen(output) + entryLen + 2 > outCap) {
            outCap *= 2;
            output = realloc(output, outCap);
        }
        strcat(output, entry);
        if (t.next != '\0') {
            size_t len = strlen(output);
            output[len] = t.next;
            output[len+1] = '\0';
        }
        char newEntry[1024];
        snprintf(newEntry, sizeof(newEntry), "%s%c", entry, t.next);
        if (dictSize >= dictCap) {
            dictCap *= 2;
            dictionary = realloc(dictionary, dictCap * sizeof(char*));
        }
        dictionary[dictSize++] = strdup(newEntry);
    }
    for (size_t i = 0; i < dictSize; i++)
        free(dictionary[i]);
    free(dictionary);
    free(tokens);
    return output;
}
