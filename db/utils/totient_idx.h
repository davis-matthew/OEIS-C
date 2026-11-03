/* totient_idx: Euler's totient phi(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_TOTIENT_IDX_H
#define OEISC_UTILS_TOTIENT_IDX_H
#include <stdint.h>
#include <stdlib.h>

/* Euler's totient for idx_t */
static idx_t totient_idx(idx_t m)
{
    if (m == 0) return 0;
    idx_t result = m;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

#endif
