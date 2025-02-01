#if !defined(CCOMPRA_LZ5_H_LOADED)
#define CCOMPRA_LZ5_H_LOADED

#include "export.h"
#include "std.h"

#if !defined(LZ5_WINDOW)
#define LZ5_WINDOW 512
#endif // LZ5_WINDOW

#if !defined(LZ5_MIN_MATCH)
#define LZ5_MIN_MATCH 3
#endif // LZ5_MIN_MATCH

CCOMPRA_EXTERN_C_BEG

CCOMPRA_PUBLIC_FUNC char *lz5_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *lz5_decompress(const char *input);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_LZ5_H_LOADED
