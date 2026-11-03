/* ipow_u64_to_seq: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U64_TO_SEQ_H
#define OEISC_UTILS_IPOW_U64_TO_SEQ_H
#include <stddef.h>
#include <stdint.h>

/* small power function using seq_t arithmetic, exponent is small (≤~45 for n≤10000) */
static seq_t ipow_u64_to_seq(uint64_t base, uint64_t exp)
{
    seq_t result = 1;
    seq_t cur    = (seq_t)base;
    while (exp > 0) {
        if (exp & 1ULL) result *= cur;
        exp >>= 1ULL;
        if (exp) cur *= cur;
    }
    return result;
}

#endif
