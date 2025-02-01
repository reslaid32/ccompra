#if !defined(CCOMPRA_LZMA_H_LOADED)
#define CCOMPRA_LZMA_H_LOADED

#include "export.h"
#include "std.h"

#if !defined(LZMA_WINDOW)
#define LZMA_WINDOW 4096
#endif // LZMA_WINDOW

#if !defined(LZMA_MIN_MATCH)
#define LZMA_MIN_MATCH 3
#endif // LZMA_MIN_MATCH

CCOMPRA_EXTERN_C_BEG

typedef struct CCOMPRA_EXPORT {
    size_t pos;
    size_t len;
    char next;
} LZMAToken;

CCOMPRA_PUBLIC_FUNC char *lzma_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *lzma_decompress(const char *decompress);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_LZMA_H_LOADED
