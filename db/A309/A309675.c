/*
 * A309675  a(n) = 4^n^2 + n!.
 * https://oeis.org/A309675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n^2) + n!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

static seq_t fact_upto(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i)
        r *= (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    /* compute 4^(n^2) + n! */
    seq_t pow4 = ipow_u64_idx(4, n * n);
    seq_t fact = fact_upto(n);
    return pow4 + fact;
}
