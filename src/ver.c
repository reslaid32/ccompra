#define CCOMPRA_LIBRARY_BUILD

#include "../include/ccompra/ver.h"

CCOMPRA_PUBLIC_FUNC ccompraver_t ccompraver(void) {
    ccompraver_t compv;
    compv.major = 1;
    compv.minor = 0;
    compv.patch = 1; /* Update (added compressed bound) */
    return compv;
}