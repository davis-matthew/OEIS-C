/* omega_idx_to_u32: the number of distinct prime factors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_OMEGA_IDX_TO_U32_H
#define OEISC_UTILS_OMEGA_IDX_TO_U32_H
#include <stdint.h>

static unsigned omega_idx_to_u32(idx_t e)
{
    unsigned cnt = 0;
    for (idx_t d = 2; d <= e / d; ++d) {
        if (e % d == 0) {
            ++cnt;
            while (e % d == 0) e /= d;
        }
    }
    if (e > 1) ++cnt;                /* remaining prime factor */
    return cnt;
}

#endif
