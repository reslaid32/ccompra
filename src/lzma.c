#define CCOMPRA_LIBRARY_BUILD

#include "../include/ccompra/lzma.h"

CCOMPRA_PUBLIC_FUNC
char* lzma_compress(const char *input) {
    size_t inLen = strlen(input);
    size_t outCap = inLen * 4 + 1;
    LZMAToken *tokens = malloc(128 * sizeof(LZMAToken));
    size_t tokenCount = 0, tokenCap = 128;
    size_t pos = 0;
    while (pos < inLen) {
        int bestLen = 0, bestPos = 0;
        size_t start = (pos >= LZMA_WINDOW) ? pos - LZMA_WINDOW : 0;
        for (size_t j = start; j < pos; j++) {
            int matchLen = 0;
            while (pos + matchLen < inLen && input[j + matchLen] == input[pos + matchLen])
                matchLen++;
            if (matchLen > bestLen) {
                bestLen = matchLen;
                bestPos = pos - j;
            }
        }
        LZMAToken t;
        t.pos = bestPos;
        t.len = bestLen;
        t.next = (pos + bestLen < inLen) ? input[pos + bestLen] : '\0';
        if (tokenCount >= tokenCap) {
            tokenCap *= 2;
            tokens = realloc(tokens, tokenCap * sizeof(LZMAToken));
        }
        tokens[tokenCount++] = t;
        pos += bestLen + 1;
    }
    /* Serialize tokens */
    char *output = malloc(outCap);
    output[0] = '\0';
    for (size_t i = 0; i < tokenCount; i++) {
        char token[64];
        snprintf(token, sizeof(token), "Z,%zu,%zu,%d;", tokens[i].pos, tokens[i].len, (int)tokens[i].next);
        if (strlen(output) + strlen(token) + 1 > outCap) {
            outCap *= 2;
            output = realloc(output, outCap);
        }
        strcat(output, token);
    }
    free(tokens);
    return output;
}

CCOMPRA_PUBLIC_FUNC
char* lzma_decompress(const char *compressed) {
    /* Deserialize tokens */
    size_t tokenCap = 128, tokenCount = 0;
    LZMAToken *tokens = malloc(tokenCap * sizeof(LZMAToken));
    const char *p = compressed;
    while (*p) {
        size_t pos, len;
        int nextInt;
        if (sscanf(p, "Z,%zu,%zu,%d;", &pos, &len, &nextInt) != 3)
            break;
        if (tokenCount >= tokenCap) {
            tokenCap *= 2;
            tokens = realloc(tokens, tokenCap * sizeof(LZMAToken));
        }
        tokens[tokenCount].pos = pos;
        tokens[tokenCount].len = len;
        tokens[tokenCount].next = (char)nextInt;
        tokenCount++;
        const char *semicolon = strchr(p, ';');
        if (!semicolon)
            break;
        p = semicolon + 1;
    }
    size_t outCap = 256, outLen = 0;
    char *output = malloc(outCap);
    output[0] = '\0';
    for (size_t i = 0; i < tokenCount; i++) {
        LZMAToken t = tokens[i];
        if (t.len > 0) {
            while (outLen + t.len >= outCap) {
                outCap *= 2;
                output = realloc(output, outCap);
            }
            size_t start = outLen - t.pos;
            for (size_t j = 0; j < t.len; j++)
                output[outLen++] = output[start + j];
        }
        if (t.next != '\0') {
            if (outLen + 1 >= outCap) {
                outCap *= 2;
                output = realloc(output, outCap);
            }
            output[outLen++] = t.next;
        }
        output[outLen] = '\0';
    }
    free(tokens);
    return output;
}

CCOMPRA_PUBLIC_FUNC
size_t lzma_compress_bound(size_t srcSz) {
    if (srcSz == 0)
        return 1;
    return srcSz + (srcSz / 3) + 128;
}
