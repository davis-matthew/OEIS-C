/* mobius_idx: the Moebius function mu(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MOBIUS_IDX_H
#define OEISC_UTILS_MOBIUS_IDX_H
#include <stdint.h>

/* Compute the Möbius function μ(k) for k >= 1. */
static int mobius_idx(idx_t k)
{
    if (k == 1) return 1;
    int cnt = 0;
    for (idx_t p = 2; p * p <= k; ++p) {
        if (k % p == 0) {
            int exp = 0;
            while (k % p == 0) {
                k /= p;
                ++exp;
                if (exp > 1) return 0;   /* squared prime factor */
            }
            ++cnt;   /* distinct prime factor */
        }
    }
    if (k > 1) ++cnt;   /* remaining prime factor */
    return (cnt % 2 == 0) ? 1 : -1;
}

#endif
