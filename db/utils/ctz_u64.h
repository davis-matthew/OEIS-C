/* ctz_u64: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_U64_H
#define OEISC_UTILS_CTZ_U64_H
#include <stdbool.h>
#include <stdint.h>

/* Count trailing zeros of a 64‑bit unsigned integer.
   Undefined when x==0, but we guard against that in a(). */
static inline int ctz_u64(uint64_t x) {
    return __builtin_ctzll(x);
}

#endif
