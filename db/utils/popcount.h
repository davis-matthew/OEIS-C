/* popcount: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_H
#define OEISC_UTILS_POPCOUNT_H
#include <stdbool.h>
#include <stdint.h>

/* Count set bits (1‑bits) of a 64‑bit unsigned integer. */
static inline int popcount(uint64_t x) {
    return __builtin_popcountll(x);
}

#endif
