#if !defined(CCOMPRA_DEFLATE_H_LOADED)
#define CCOMPRA_DEFLATE_H_LOADED

#include "export.h"
#include "std.h"

// #include "lz77.h"
#include "huffman.h"

typedef HuffmanCompressed DeflateCompressed;

CCOMPRA_EXTERN_C_BEG

CCOMPRA_PUBLIC_FUNC DeflateCompressed *deflate_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *deflate_decompress(DeflateCompressed *compressed);
CCOMPRA_PUBLIC_FUNC size_t deflate_compress_bound(size_t srcSz);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_DEFLATE_H_LOADED
