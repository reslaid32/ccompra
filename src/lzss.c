#define CCOMPRA_LIBRARY_BUILD

#include "../include/ccompra/lzss.h"

CCOMPRA_PUBLIC_FUNC
char* lzss_compress(const char *input, size_t windowSize) {
    size_t inLen = strlen(input);
    size_t tokenCap = 128;
    LZSSToken *tokens = malloc(tokenCap * sizeof(LZSSToken));
    size_t tokenCount = 0;
    size_t pos = 0;
    while (pos < inLen) {
        size_t bestLen = 0, bestOffset = 0;
        size_t start = (pos >= windowSize) ? pos - windowSize : 0;
        for (size_t j = start; j < pos; j++) {
            size_t matchLen = 0;
            while (pos + matchLen < inLen && input[j + matchLen] == input[pos + matchLen])
                matchLen++;
            if (matchLen > bestLen) {
                bestLen = matchLen;
                bestOffset = pos - j;
            }
        }
        if (bestLen >= 2) {
            LZSSToken t = {0, '\0', bestOffset, bestLen};
            tokens[tokenCount++] = t;
            pos += bestLen;
        } else {
            LZSSToken t = {1, input[pos], 0, 0};
            tokens[tokenCount++] = t;
            pos++;
        }
        if (tokenCount >= tokenCap) {
            tokenCap *= 2;
            tokens = realloc(tokens, tokenCap * sizeof(LZSSToken));
        }
    }
    size_t outCap = tokenCount * 32 + 1;
    char *output = malloc(outCap);
    output[0] = '\0';
    for (size_t i = 0; i < tokenCount; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d,%d,%zu,%zu;", tokens[i].isLiteral, (int)tokens[i].literal, tokens[i].offset, tokens[i].length);
        if (strlen(output) + strlen(buf) + 1 > outCap) {
            outCap *= 2;
            output = realloc(output, outCap);
        }
        strcat(output, buf);
    }
    free(tokens);
    return output;
}

CCOMPRA_PUBLIC_FUNC
char* lzss_decompress(const char *compressed) {
    size_t tokenCap = 128, tokenCount = 0;
    LZSSToken *tokens = malloc(tokenCap * sizeof(LZSSToken));
    const char *p = compressed;
    while (*p) {
        int isLit, lit;
        size_t offset, length;
        if (sscanf(p, "%d,%d,%zu,%zu;", &isLit, &lit, &offset, &length) != 4)
            break;
        if (tokenCount >= tokenCap) {
            tokenCap *= 2;
            tokens = realloc(tokens, tokenCap * sizeof(LZSSToken));
        }
        tokens[tokenCount].isLiteral = isLit;
        tokens[tokenCount].literal = (char)lit;
        tokens[tokenCount].offset = offset;
        tokens[tokenCount].length = length;
        tokenCount++;
        const char *semicolon = strchr(p, ';');
        if (!semicolon) break;
        p = semicolon + 1;
    }
    size_t outCap = 256, outLen = 0;
    char *output = malloc(outCap);
    output[0] = '\0';
    for (size_t i = 0; i < tokenCount; i++) {
        if (tokens[i].isLiteral) {
            if (outLen + 1 >= outCap) {
                outCap *= 2;
                output = realloc(output, outCap);
            }
            output[outLen++] = tokens[i].literal;
            output[outLen] = '\0';
        } else {
            size_t start = outLen - tokens[i].offset;
            while (outLen + tokens[i].length >= outCap) {
                outCap *= 2;
                output = realloc(output, outCap);
            }
            for (size_t j = 0; j < tokens[i].length; j++)
                output[outLen++] = output[start + j];
            output[outLen] = '\0';
        }
    }
    free(tokens);
    return output;
}
