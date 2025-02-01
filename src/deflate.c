#define CCOMPRA_LIBRARY_BUILD

#include "../include/ccompra/lz77.h"
#include "../include/ccompra/deflate.h"

CCOMPRA_PUBLIC_FUNC
HuffmanCompressed* deflate_compress(const char *input) {
    char *lz77Tokens = lz77_compress(input);
    HuffmanCompressed *huffOut = huffman_compress(lz77Tokens);
    free(lz77Tokens);
    return huffOut;
}

CCOMPRA_PUBLIC_FUNC
char* deflate_decompress(HuffmanCompressed *compressed) {
    char *lz77Serialized = huffman_decompress(compressed);
    char *output = lz77_decompress(lz77Serialized);
    free(lz77Serialized);
    return output;
}