/*
 * A396398  a(n) = r(n,n), where r(1,k)=f(k)=round(k*c), r(n,k)=f(r(n-1,k)),
 *          c=sqrt(2).
 * https://oeis.org/A396398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n,n) where r(1,k) = round(k * sqrt(2)), r(n,k) =
 *            round(r(n-1,k) * sqrt(2)), c = sqrt(2)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 21, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"

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

/* Perform round(x * sqrt(2)) using integer arithmetic.
   The function works for any size of seq_t. */
static seq_t round_sqrt2(seq_t x)
{
    /* calculate 2 * x * x */
    seq_t t = x * x;
    t = mul_small(t, 2);

    /* nearest integer to sqrt(t) */
    seq_t f  = floor_sqrt(t);
    seq_t sq1 = f * f;
    seq_t diff1 = t - sq1;

    seq_t f1 = f + 1;
    seq_t sq2 = f1 * f1;
    seq_t diff2 = sq2 - t;

    /* round up if nearer to f+1, otherwise keep f */
    return (diff1 < diff2) ? f : f1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* k = n */
    for (idx_t i = 0; i < n; ++i) /* apply r(n,n) n times */
    {
        x = round_sqrt2(x);
    }
    return x;
}
