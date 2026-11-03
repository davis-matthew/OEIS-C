/*
 * A327205  a(n) = [(2n+2)r] - [(n+2)r] - [nr], where [ ] = floor and r =
 *          sqrt(2).
 * https://oeis.org/A327205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n+2)*sqrt(2)) - floor((n+2)*sqrt(2)) -
 *            floor(n*sqrt(2))
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9940 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses integer square root of 2*k^2 to obtain floor(k*sqrt(2)) exactly. */
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
    seq_t k_n  = (seq_t)n;                        /* k = n */
    seq_t k_n2 = k_n + 2;                         /* k = n+2 */
    seq_t k_2n = (seq_t)(2 * n + 2);              /* k = 2n+2 */

    /* Compute floor(k*sqrt(2)) as floor(sqrt(2*k^2)) */
    seq_t sqrt_n  = floor_sqrt( (seq_t)( (__int128)2 * k_n  * k_n ) );
    seq_t sqrt_n2 = floor_sqrt( (seq_t)( (__int128)2 * k_n2 * k_n2 ) );
    seq_t sqrt_2n = floor_sqrt( (seq_t)( (__int128)2 * k_2n * k_2n ) );

    return sqrt_2n - sqrt_n2 - sqrt_n;
}
