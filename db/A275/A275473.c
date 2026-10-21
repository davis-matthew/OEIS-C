/*
 * A275473  Least k such that d(k*n) = k, or 0 if no such k exists (d =
 *          A000005).
 * https://oeis.org/A275473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least k such that d(k*n) = k, or 0 if none found.  The search is bounded
   by k ≤ 4*n, which follows from the inequality d(m) ≤ 2*√m for m>1. */
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    uint64_t nn = (uint64_t)n;
    uint64_t maxk = 4 * nn;
    for (uint64_t k = 1; k <= maxk; ++k) {
        uint64_t m = nn * k;
        seq_t d = divisor_count_u64(m);
        if ((seq_t)k == d) return d;          /* k = d(m) found */
    }
    return 0;                                 /* no such k up to 4*n */
}
