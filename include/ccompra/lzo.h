#if !defined(CCOMPRA_LZO_H_LOADED)
#define CCOMPRA_LZO_H_LOADED

#include "export.h"
#include "std.h"

#if !defined(LZO_WINDOW)
#define LZO_WINDOW 256
#endif // LZO_WINDOW

#if !defined(LZO_MIN_MATCH)
#define LZO_MIN_MATCH 3
#endif // LZO_MIN_MATCH

CCOMPRA_EXTERN_C_BEG

CCOMPRA_PUBLIC_FUNC char *lzo_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *lzo_decompress(const char *input);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_LZO_H_LOADED
