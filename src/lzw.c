#define CCOMPRA_LIBRARY_BUILD

#include "../include/ccompra/lzw.h"

CCOMPRA_PUBLIC_FUNC
LZWResult lzw_compress(const char *input) {
    size_t inLen = strlen(input);
    size_t outCap = 128;
    int *result = malloc(outCap * sizeof(int));
    size_t resultLen = 0;

    size_t dictCap = 256;
    size_t dictSize = 0;
    char **dictionary = malloc(dictCap * sizeof(char *));
    
    // Initialize dictionary with all 256 single-character strings.
    for (size_t i = 0; i < 256; i++) {
        char str[2] = { (char)i, '\0' };
        dictionary[dictSize++] = strdup(str);
    }

    // Dynamic buffer for current pattern
    char *w = strdup("");
    for (size_t i = 0; i < inLen; i++) {
        char c[2] = { input[i], '\0' };
        
        // Allocate combined buffer for wc
        size_t wcLen = strlen(w) + strlen(c) + 1;
        char *wc = malloc(wcLen);
        snprintf(wc, wcLen, "%s%s", w, c);

        // Search for wc in dictionary
        int found = -1;
        for (size_t j = 0; j < dictSize; j++) {
            if (strcmp(dictionary[j], wc) == 0) { 
                found = (int)j;
                break;
            }
        }

        if (found != -1) {
            free(w);
            w = wc;
        } else {
            // Find the code for w
            int code = -1;
            for (size_t j = 0; j < dictSize; j++) {
                if (strcmp(dictionary[j], w) == 0) { 
                    code = (int)j; 
                    break; 
                }
            }
            if (resultLen >= outCap) {
                outCap *= 2;
                result = realloc(result, outCap * sizeof(int));
            }
            result[resultLen++] = code;

            // Add wc to the dictionary
            if (dictSize >= dictCap) {
                dictCap *= 2;
                dictionary = realloc(dictionary, dictCap * sizeof(char *));
            }
            dictionary[dictSize++] = strdup(wc);

            free(w);
            w = strdup(c);
            free(wc);
        }
    }

    // Output the code for w if non-empty
    if (strlen(w) > 0) {
        int code = -1;
        for (size_t j = 0; j < dictSize; j++) {
            if (strcmp(dictionary[j], w) == 0) { 
                code = (int)j;
                break;
            }
        }
        if (resultLen >= outCap) {
            outCap *= 2;
            result = realloc(result, outCap * sizeof(int));
        }
        result[resultLen++] = code;
    }

    // Free dictionary memory
    for (size_t i = 0; i < dictSize; i++)
        free(dictionary[i]);
    free(dictionary);
    free(w);

    LZWResult res;
    res.codes = result;
    res.length = resultLen;
    return res;
}


CCOMPRA_PUBLIC_FUNC
char* serializeLZWResult(LZWResult res) {
    // Allocate an output buffer (each integer may be up to 11 characters plus comma)
    size_t cap = res.length * 12 + 1;
    char *output = malloc(cap);
    output[0] = '\0';
    for (size_t i = 0; i < res.length; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d,", res.codes[i]);
        strcat(output, buf);
    }
    return output;
}

CCOMPRA_PUBLIC_FUNC
LZWResult deserializeLZWResult(const char *str) {
    LZWResult res;
    size_t cap = 128;
    res.codes = malloc(cap * sizeof(int));
    res.length = 0;
    const char *p = str;
    while (*p) {
        int code;
        if (sscanf(p, "%d,", &code) != 1)
            break;
        if (res.length >= cap) {
            cap *= 2;
            res.codes = realloc(res.codes, cap * sizeof(int));
        }
        res.codes[res.length++] = code;
        const char *comma = strchr(p, ',');
        if (!comma) break;
        p = comma + 1;
    }
    return res;
}

CCOMPRA_PUBLIC_FUNC
char *lzw_decompress(LZWResult res) {
    size_t dictCap = 256;
    
    // Initialize dictionary with 256 single-character strings.
    char **dictionary = malloc(dictCap * sizeof(char *));
    size_t dictSize = 256;

    for (size_t i = 0; i < 256; i++) {
        char str[2] = { (char)i, '\0' };
        dictionary[i] = strdup(str);
    }

    // Handle the first code separately.
    int firstCode = res.codes[0];
    char *w = strdup(dictionary[firstCode]);

    // Allocate initial output buffer.
    size_t outCap = 256;
    char *output = malloc(outCap);
    if (!output) return NULL;
    output[0] = '\0';
    strcat(output, w);

    // Process the rest of the codes.
    for (size_t i = 1; i < res.length; i++) {
        int k = res.codes[i];
        char *entry;
        
        if (k < (int)dictSize) {
            entry = strdup(dictionary[k]);
        } else {
            // Handle special case: k not found in dictionary.
            size_t entryLen = strlen(w) + 2;
            entry = malloc(entryLen);
            snprintf(entry, entryLen, "%s%c", w, w[0]);
        }

        // Ensure output buffer is large enough.
        size_t requiredSize = strlen(output) + strlen(entry) + 1;
        if (requiredSize > outCap) {
            while (outCap < requiredSize) {
                outCap *= 2;
            }
            output = realloc(output, outCap);
        }
        strcat(output, entry);

        // Add new entry to dictionary: w + first character of entry.
        size_t newEntryLen = strlen(w) + 2;
        char *newEntry = malloc(newEntryLen);
        snprintf(newEntry, newEntryLen, "%s%c", w, entry[0]);

        if (dictSize >= dictCap) {
            dictCap *= 2;
            dictionary = realloc(dictionary, dictCap * sizeof(char *));
        }
        dictionary[dictSize++] = newEntry;

        free(w);
        w = entry;
    }

    // Cleanup
    free(w);
    for (size_t i = 0; i < dictSize; i++) {
        free(dictionary[i]);
    }
    free(dictionary);

    return output;
}
