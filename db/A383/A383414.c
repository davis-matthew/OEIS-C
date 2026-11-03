/*
 * A383414  Array read by ascending antidiagonals: A(n,k) = 4^n*(8*k + 7).
 * https://oeis.org/A383414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 4^i * (8*j + 7) where (i,j) are the antidiagonal
 *            coordinates of index n
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    seq_t v   = ((seq_t)8 * n) + 1;          /* 8*n + 1 as seq_t */
    seq_t s   = floor_sqrt(v);              /* s = floor(sqrt(8*n+1)) */
    idx_t d   = (idx_t)((s - 1) / 2);       /* diagonal index */

    idx_t start = d * (d + 1) / 2;          /* number of terms before diagonal d */
    idx_t t     = n - start;                /* offset within the current diagonal */

    idx_t i = d - t;                        /* row index (n_coord) */
    idx_t j = t;                            /* column index (k_coord) */

    seq_t term = pow4(i) * ((seq_t)8 * j + 7);
    return term;
}
