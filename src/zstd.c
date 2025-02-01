#define CCOMPRA_LIBRARY_BUILD

#include "../include/ccompra/lz77.h"
#include "../include/ccompra/zstd.h"

CCOMPRA_PUBLIC_FUNC
FSECompressed* zstd_compress(const char *input) {
    char *lz77Tokens = lz77_compress(input);
    FSECompressed *fseOut = fse_compress(lz77Tokens);
    free(lz77Tokens);
    return fseOut;
}

CCOMPRA_PUBLIC_FUNC
char* zstd_decompress(FSECompressed *compressed) {
    char *lz77Serialized = fse_decompress(compressed);
    char *output = lz77_decompress(lz77Serialized);
    free(lz77Serialized);
    return output;
}