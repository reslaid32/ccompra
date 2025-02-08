#define CCOMPRA_LIBRARY_BUILD

#include "../include/ccompra/fse.h"


CCOMPRA_PRIVATE_FUNC
int ceil_log2(int n) {
    int bits = 0;
    while ((1 << bits) < n) bits++;
    return bits;
}

CCOMPRA_PUBLIC_FUNC
FSECompressed* fse_compress(const char *input) {
    size_t inLen = strlen(input);
    if (inLen == 0)
        return NULL; // nothing to compress

    int freq[256] = {0};
    for (size_t i = 0; i < inLen; i++) {
        freq[(unsigned char)input[i]]++;
    }
    char unique[256];
    int nUnique = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0)
            unique[nUnique++] = (char)i;
    }
    // Determine how many bits are needed for each symbol
    int bitsPerSymbol = (nUnique > 1) ? ceil_log2(nUnique) : 1;

    // Build header string: "<nUnique>;<uniqueSymbols>;"
    char headerBuf[512];
    int headerWritten = snprintf(headerBuf, sizeof(headerBuf), "%d;", nUnique);
    // Write exactly nUnique characters (the unique symbol table)
    for (int i = 0; i < nUnique; i++) {
        headerWritten += snprintf(headerBuf + headerWritten,
                                  sizeof(headerBuf) - headerWritten,
                                  "%c", unique[i]);
    }
    snprintf(headerBuf + headerWritten, sizeof(headerBuf) - headerWritten, ";");
    char *header = strdup(headerBuf);

    // Compute total bits and bytes required for the packed data
    int totalBits = (int)(inLen * bitsPerSymbol);
    size_t totalBytes = (totalBits + 7) / 8; // round up to whole bytes
    unsigned char *data = malloc(totalBytes);
    if (!data) {
        free(header);
        return NULL;
    }
    memset(data, 0, totalBytes);

    // Pack each input symbol into the data buffer.
    // For each symbol, find its index in the unique table,
    // then write 'bitsPerSymbol' bits (MSB first) into 'data'.
    int bitPos = 0;
    for (size_t i = 0; i < inLen; i++) {
        int idx = 0;
        for (int j = 0; j < nUnique; j++) {
            if (input[i] == unique[j]) {
                idx = j;
                break;
            }
        }
        for (int b = bitsPerSymbol - 1; b >= 0; b--) {
            int bit = (idx >> b) & 1;
            size_t byteIndex = bitPos / 8;
            int bitIndex = 7 - (bitPos % 8);  // fill from MSB to LSB
            if (bit)
                data[byteIndex] |= (1 << bitIndex);
            bitPos++;
        }
    }

    // Allocate and populate the FSECompressed structure.
    FSECompressed *comp = malloc(sizeof(FSECompressed));
    if (!comp) {
        free(header);
        free(data);
        return NULL;
    }
    comp->header = header;
    comp->bitLen = totalBits;
    comp->data = data;
    comp->dataLen = totalBytes;
    return comp;
}

CCOMPRA_PUBLIC_FUNC
char* fse_decompress(const FSECompressed *comp) {
    if (!comp)
        return NULL;

    // Make a copy of the header so we can parse it.
    char *headerCopy = strdup(comp->header);
    if (!headerCopy)
        return NULL;
    
    // Find the first semicolon to parse nUnique.
    char *p = headerCopy;
    char *semicolon = strchr(p, ';');
    if (!semicolon) {
        free(headerCopy);
        return NULL;
    }
    *semicolon = '\0';
    int nUnique = atoi(p);
    p = semicolon + 1; // p now points to the unique symbol table
    
    // Copy exactly nUnique characters as the unique table.
    char unique[256];
    if (nUnique > 0) {
        memcpy(unique, p, nUnique);
    }
    unique[nUnique] = '\0';
    
    // Skip over the unique symbol table and the following semicolon.
    p += nUnique;
    if (*p != ';') {
        free(headerCopy);
        return NULL;
    }
    // p++; // not needed further
    
    free(headerCopy);
    int bitsPerSymbol = (nUnique > 1) ? ceil_log2(nUnique) : 1;
    size_t nSymbols = comp->bitLen / bitsPerSymbol;
    char *output = malloc(nSymbols + 1);
    if (!output)
        return NULL;

    int bitPos = 0;
    for (size_t i = 0; i < nSymbols; i++) {
        int idx = 0;
        for (int b = bitsPerSymbol - 1; b >= 0; b--) {
            size_t byteIndex = bitPos / 8;
            int bitIndex = 7 - (bitPos % 8);
            int bit = (comp->data[byteIndex] >> bitIndex) & 1;
            idx |= (bit << b);
            bitPos++;
        }
        output[i] = (idx < nUnique) ? unique[idx] : '?';
    }
    output[nSymbols] = '\0';
    return output;
}

CCOMPRA_PUBLIC_FUNC
size_t fse_compress_bound(size_t srcSz) {
    return srcSz + (srcSz >> 7) + 1;
}
