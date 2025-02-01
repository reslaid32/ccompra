#if !defined(CCOMPRA_LZ77_H_LOADED)
#define CCOMPRA_LZ77_H_LOADED

#include "export.h"
#include "std.h"

#if !defined(LZ77_WINDOW)
#define LZ77_WINDOW 1024
#endif // LZ77_WINDOW

#if !defined(LZ77_MIN_MATCH)
#define LZ77_MIN_MATCH 3
#endif // LZ77_MIN_MATCH

CCOMPRA_EXTERN_C_BEG

CCOMPRA_PUBLIC_FUNC char *lz77_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *lz77_decompress(const char *compressed);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_LZ77_H_LOADED
