#if !defined(CCOMRPA_LZW_H_LOADED)
#define CCOMRPA_LZW_H_LOADED

#include "export.h"
#include "std.h"

CCOMPRA_EXTERN_C_BEG

typedef struct CCOMPRA_EXPORT {
    int *codes;
    size_t length;
} LZWResult;

CCOMPRA_PUBLIC_FUNC LZWResult lzw_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *serializeLZWResult(LZWResult res);
CCOMPRA_PUBLIC_FUNC LZWResult deserializeLZWResult(const char *str);
CCOMPRA_PUBLIC_FUNC char *lzw_decompress(LZWResult res);

CCOMPRA_EXTERN_C_END

#endif // CCOMRPA_LZW_H_LOADED
