/* icbrt_seq: floor(cbrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ICBRT_SEQ_H
#define OEISC_UTILS_ICBRT_SEQ_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer cube root: floor(cbrt(m)) for non‑negative m. */
static seq_t icbrt_seq(seq_t m)
{
    seq_t lo = 0, hi = 1;

    /* Find an upper bound hi such that hi^3 > m */
    while (1) {
        seq_t hi_sq = hi * hi;
        if (hi_sq == 0 || hi > m / hi_sq)
            break;
        hi <<= 1;
    }
    lo = hi / 2;                       /* previous hi satisfies lo^3 <= m */

    /* Binary search for the floor cube root */
    while (lo < hi) {
        seq_t mid = lo + (hi - lo + 1) / 2;      /* upper mid */
        seq_t mid_sq = mid * mid;
        if (mid_sq == 0 || mid > m / mid_sq) {
            hi = mid - 1;
            continue;
        }
        seq_t mid_cu = mid_sq * mid;
        if (mid_cu > m)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

#endif
