/* triangular_u64: n(n+1)/2.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_TRIANGULAR_U64_H
#define OEISC_UTILS_TRIANGULAR_U64_H
#include <stdint.h>

static inline uint64_t triangular_u64(uint64_t i) {
    /* Return i*(i+1)/2 using 128-bit arithmetic to avoid overflow. */
    return (uint64_t)(((__uint128_t)i * (i + 1)) >> 1);
}

#endif
