/*
 * A276886  Sums-complement of the Beatty sequence for 2 + phi.
 * https://oeis.org/A276886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor((n-1)*phi) - n + 2, phi = (1+sqrt(5))/2
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
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
    /* a(1) is the first term; for n==0 we return 0 (unused). */
    if (n == 0) return (seq_t)0;

    /* k = n-1.  Use idx_t for intermediate to avoid overflow. */
    const idx_t k_idx = n - 1;
    /* Compute floor(k*sqrt(5)) as floor(sqrt(5*k*k)). */
    const idx_t t_idx = 5 * k_idx * k_idx;          /* 5*k^2  -- idx_t safe  */
    const seq_t t = (seq_t)t_idx;                   /* promote to seq_t */
    const seq_t s = floor_sqrt(t);                  /* floor(k*sqrt(5)) */

    /* k as seq_t for final calculation */
    const seq_t k = (seq_t)k_idx;
    /* floor((k + s)/2) is floor(k*phi) */
    const seq_t floor_k_phi = (k + s) / 2;

    /* a(n) = 2*floor_k_phi - n + 2 */
    return 2 * floor_k_phi - (seq_t)n + 2;
}
