#if !defined(CCOMPRA_DEFLATE_H_LOADED)
#define CCOMPRA_DEFLATE_H_LOADED

#include "export.h"
#include "std.h"

// #include "lz77.h"
#include "huffman.h"

CCOMPRA_EXTERN_C_BEG

CCOMPRA_PUBLIC_FUNC HuffmanCompressed *deflate_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *deflate_decompress(HuffmanCompressed *compressed);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_DEFLATE_H_LOADED
