#define CCOMPRA_LIBRARY_BUILD

#include "../include/ccompra/lzo.h"

CCOMPRA_PUBLIC_FUNC
char* lzo_compress(const char *input) {
    size_t inLen = strlen(input);
    size_t outCap = inLen * 4 + 1;
    char *output = malloc(outCap);
    output[0] = '\0';
    size_t pos = 0;
    while (pos < inLen) {
        int bestMatchLen = 0;
        int bestOffset = 0;
        size_t start = (pos >= LZO_WINDOW) ? pos - LZO_WINDOW : 0;
        for (size_t j = start; j < pos; j++) {
            int matchLen = 0;
            while (pos + matchLen < inLen && input[j + matchLen] == input[pos + matchLen])
                matchLen++;
            if (matchLen > bestMatchLen) {
                bestMatchLen = matchLen;
                bestOffset = pos - j;
            }
        }
        if (bestMatchLen >= LZO_MIN_MATCH) {
            char token[64];
            snprintf(token, sizeof(token), "M,%d,%d;", bestOffset, bestMatchLen);
            if (strlen(output) + strlen(token) + 1 > outCap) {
                outCap *= 2;
                output = realloc(output, outCap);
            }
            strcat(output, token);
            pos += bestMatchLen;
        } else {
            char token[16];
            snprintf(token, sizeof(token), "L,%c;", input[pos]);
            if (strlen(output) + strlen(token) + 1 > outCap) {
                outCap *= 2;
                output = realloc(output, outCap);
            }
            strcat(output, token);
            pos++;
        }
    }
    return output;
}

CCOMPRA_PUBLIC_FUNC
char* lzo_decompress(const char *compressed) {
    size_t outCap = 256;
    char *output = malloc(outCap);
    size_t outLen = 0;
    output[0] = '\0';
    const char *p = compressed;
    while (*p) {
        if (*p == 'M') {
            int offset, length;
            if (sscanf(p, "M,%d,%d;", &offset, &length) != 2)
                break;
            while (outLen + length >= outCap) {
                outCap *= 2;
                output = realloc(output, outCap);
            }
            int start = (int)outLen - offset;
            for (int i = 0; i < length; i++)
                output[outLen++] = output[start + i];
            output[outLen] = '\0';
        } else if (*p == 'L') {
            char lit;
            if (sscanf(p, "L,%c;", &lit) != 1)
                break;
            if (outLen + 1 >= outCap) {
                outCap *= 2;
                output = realloc(output, outCap);
            }
            output[outLen++] = lit;
            output[outLen] = '\0';
        }
        const char *semicolon = strchr(p, ';');
        if (!semicolon) break;
        p = semicolon + 1;
    }
    return output;
}
