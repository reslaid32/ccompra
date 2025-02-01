#if !defined(CCOMPRA_ZSTD_H_LOADED)
#define CCOMPRA_ZSTD_H_LOADED

#include "export.h"
#include "std.h"

// #include "lz77.h"
#include "fse.h"

CCOMPRA_EXTERN_C_BEG

CCOMPRA_PUBLIC_FUNC FSECompressed *zstd_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *zstd_decompress(FSECompressed *compressed);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_ZSTD_H_LOADED
