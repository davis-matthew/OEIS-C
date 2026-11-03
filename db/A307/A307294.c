/*
 * A307294  If n is even, a(n) = A000201(n/2+1), otherwise a(n) =
 *          A000201((n-1)/2+1) + 1.
 * https://oeis.org/A307294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000201(n/2+1) if n is even, else a(n) =
 *            A000201((n-1)/2+1)+1. A000201(k) = floor(k * φ) (φ = (1+√5)/2)
 * Range:     u8 n <= 5, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 66 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* k = floor(n/2) + 1  (k >= 1) */
    idx_t k_idx = (n >> 1) + 1;
    seq_t k = (seq_t)k_idx;

    /* Compute floor(k * φ) = floor((floor(k * √5) + k) / 2)  */
    seq_t ksq  = k * k;                     /* k²   */
    seq_t x    = (seq_t)5 * ksq;            /* 5 * k² */
    seq_t sq    = floor_sqrt(x);            /* floor(k√5)  */
    seq_t num   = sq + k;                   /* sq + k      */
    seq_t val   = num / 2;                  /* floor((sq + k)/2) = floor(kφ) */

    /* If n is odd, add one. */
    if (n & 1U)
        return val + 1;
    else
        return val;
}
