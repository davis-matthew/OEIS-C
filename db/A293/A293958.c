/*
 * A293958  Smallest odd prime divisor of (2n+1)^2 + 1.
 * https://oeis.org/A293958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd prime divisor of (2n+1)^2 + 1
 * Range:     u8 n <= 5, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9730 (2026-11-01)
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

/* Compute a(n) for the OEIS sequence A293958.
 * It returns the smallest odd prime divisor of (2n+1)^2 + 1.
 */
seq_t a(idx_t n)
{
    /* (2n+1) */
    seq_t two_n_plus1 = (seq_t)n * 2 + 1;
    /* (2n+1)^2 + 1 */
    seq_t m = two_n_plus1 * two_n_plus1 + 1;
    /* m is even; remove the factor 2 to get the odd part. */
    seq_t q = m >> 1;                /* m / 2 */
    /* Find the smallest odd prime divisor of q. */
    seq_t limit = floor_sqrt(q);     /* floor(sqrt(q)) */
    for (seq_t p = 3; p <= limit; p += 2) {
        if ((q % p) == 0)
            return p;
    }
    /* If no divisor was found, q itself is prime (odd). */
    return q;
}
