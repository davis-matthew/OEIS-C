/*
 * A396396  a(n) = r(n,n), where r(1,k)=f(k)=floor(k*c), r(n,k)=f(r(n-1,k)),
 *          c=sqrt(2).
 * https://oeis.org/A396396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n,n) with r(1,k)=⌊k√2⌋, r(m,k)=⌊r(m‑1,k)√2⌋. Here
 *            f(k)=⌊k√2⌋ computed as ⌊√(2k²)⌋ using integer square‑root
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

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

/* Compute the n-th term of A396396.  idx_t is an unsigned 64‑bit index,
   seq_t is an unsigned integer type (u8..u128 or big integer) */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;           /* start value k = n */
    for (idx_t i = 0; i < n; ++i) {
        seq_t t = x * x;          /* x² */
        t = t + t;                /* 2·x² */
        x = floor_sqrt(t);        /* ⌊√(2x²)⌋ = ⌊x√2⌋ */
    }
    return x;
}
