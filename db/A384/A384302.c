/*
 * A384302  a(n) = Product_{k=0..2*n-1} (3*n+k).
 * https://oeis.org/A384302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{k=0}^{2*n-1} (3*n + k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

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

seq_t a(idx_t n) {
    /* Compute a(n) = product_{k=0}^{2*n-1} (3*n + k) */
    idx_t two_n = 2 * n;
    seq_t res = (seq_t)1;
    idx_t start = 3 * n;
    for (idx_t i = 0; i < two_n; ++i) {
        /* mul_small multiplies a seq_t by a small unsigned int */
        res = mul_small(res, (unsigned int)(start + i));
    }
    return res;
}
