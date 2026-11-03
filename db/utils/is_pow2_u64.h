/* is_pow2_u64: whether n is a power of 2.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_POW2_U64_H
#define OEISC_UTILS_IS_POW2_U64_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*--- helpers ----------------------------------------------------------*/
static inline bool is_pow2_u64(uint64_t e)
{
    /* e >= 1 for all candidates; 1 is 2^0, hence a power of two */
    return e && !(e & (e - 1));
}

#endif
