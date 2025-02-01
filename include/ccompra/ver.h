#if !defined(CCOMPRA_VER_H_LOADED)
#define CCOMPRA_VER_H_LOADED

#include "export.h"

CCOMPRA_EXTERN_C_BEG

typedef struct CCOMPRA_EXPORT {
    unsigned major, minor, patch;
} ccompraver_t;

CCOMPRA_PUBLIC_FUNC ccompraver_t ccompraver(void);

CCOMPRA_EXTERN_C_END

#endif // CCOMPRA_VER_H_LOADED
