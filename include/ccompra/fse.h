#if !defined(CCOMPRA_FSE_H_LOADED)
#define CCOMPRA_FSE_H_LOADED

#include "export.h"
#include "std.h"

CCOMPRA_EXTERN_C_BEG

typedef struct CCOMPRA_EXPORT {
    char *header;
    int bitLen;
    unsigned char *data;
    size_t dataLen;
} FSECompressed;

CCOMPRA_PUBLIC_FUNC FSECompressed *fse_compress(const char *input);
CCOMPRA_PUBLIC_FUNC char *fse_decompress(const FSECompressed *compressed);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_FSE_H_LOADED
