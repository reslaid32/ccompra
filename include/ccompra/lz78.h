#if !defined(CCOMPRA_LZ78_H_LOADED)
#define CCOMPRA_LZ78_H_LOADED

#include "export.h"
#include "std.h"

typedef struct CCOMPRA_EXPORT {
    size_t index;
    char next;
} LZ78Token;

CCOMPRA_EXTERN_C_BEG

CCOMPRA_PUBLIC_FUNC char *lz78_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *lz78_decompress(const char *compress);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_LZ78_H_LOADED
