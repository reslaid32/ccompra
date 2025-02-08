#if !defined(CCOMPRA_HUFFMAN_H_LOADED)
#define CCOMPRA_HUFFMAN_H_LOADED

#include "export.h"
#include "std.h"

CCOMPRA_EXTERN_C_BEG

typedef struct CCOMPRA_EXPORT {
    char *freqStr;          // Serialized frequency table (ASCII)
    int bitLen;             // Number of valid bits in the packed data
    unsigned char *data;    // Packed binary data
    size_t dataLen;         // Number of bytes in 'data'
} HuffmanCompressed;

CCOMPRA_PUBLIC_FUNC HuffmanCompressed *huffman_compress(const char *text);
CCOMPRA_PUBLIC_FUNC char *huffman_decompress(const HuffmanCompressed *compressed);
CCOMPRA_PUBLIC_FUNC size_t huffman_compress_bound(size_t srcSz);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_HUFFMAN_H_LOADED
