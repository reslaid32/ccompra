#if !defined(CCOMPRA_LZ4_H_LOADED)
#define CCOMPRA_LZ4_H_LOADED

#include "export.h"
#include "std.h"

#if !defined(LZ4_WINDOW)
#define LZ4_WINDOW 64
#endif // LZ4_WINDOW

#if !defined(LZ4_MIN_MATCH)
#define LZ4_MIN_MATCH 4
#endif // LZ4_MIN_MATCH

CCOMPRA_EXTERN_C_BEG

CCOMPRA_PUBLIC_FUNC char *lz4_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *lz4_decompress(const char *compressed);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_LZ4_H_LOADED
