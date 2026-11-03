/*
 * A305623  Number of chiral pairs of rows of n colors with exactly 3
 *          different colors.
 * https://oeis.org/A305623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * (S2(n,3) - S2(ceil(n/2),3))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Compute S2(n,3) = (3^n - 3*2^n + 3) / 6 */
    seq_t two   = ipow_u64_idx(2, n);
    seq_t three = ipow_u64_idx(3, n);
    seq_t s2_n  = (three - mul_small(two, 3) + 3) / 6;

    /* Compute S2(ceil(n/2),3) */
    idx_t m = (n + 1) / 2;
    seq_t two_m   = ipow_u64_idx(2, m);
    seq_t three_m = ipow_u64_idx(3, m);
    seq_t s2_m    = (three_m - mul_small(two_m, 3) + 3) / 6;

    /* Final result */
    seq_t diff    = s2_n - s2_m;
    return mul_small(diff, 3);
}
