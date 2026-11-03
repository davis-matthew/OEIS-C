/* ipow_u64: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U64_H
#define OEISC_UTILS_IPOW_U64_H
#include <stdint.h>

/* Compute base^exp for arbitrary-precision seq_t */
static seq_t ipow_u64(seq_t base, unsigned long long exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

#endif
