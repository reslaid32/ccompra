#if !defined(CCOMPRA_LZSS_H_LOADED)
#define CCOMPRA_LZSS_H_LOADED

#include "export.h"
#include "std.h"

CCOMPRA_EXTERN_C_BEG

typedef struct CCOMPRA_EXPORT {
    int isLiteral;
    char literal;
    size_t offset;
    size_t length;
} LZSSToken;

CCOMPRA_PUBLIC_FUNC char *lzss_compress(const char *input, size_t windowSize);
CCOMPRA_PUBLIC_FUNC char *lzss_decompress(const char *compressed);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_LZSS_H_LOADED
