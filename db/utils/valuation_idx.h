/* valuation_idx: the exponent of p in n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_VALUATION_IDX_H
#define OEISC_UTILS_VALUATION_IDX_H
#include <stdint.h>

/* Return exponent of prime p in integer x (x>=0). For x==0 the result is 0. */
static idx_t valuation_idx(idx_t x, idx_t p)
{
    idx_t e = 0;
    if (x == 0)
        return 0;
    while ((x % p) == 0) {
        x /= p;
        ++e;
    }
    return e;
}

#endif
