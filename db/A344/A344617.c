/*
 * A344617  Sign of the alternating sum of the prime indices of n.
 * https://oeis.org/A344617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if n is a perfect square; otherwise a(n) =
 *            (-1)^(Ω(n)-1) where Ω(n) = total number of prime factors of n
 *            with multiplicity (A001222)
 * Range:     i8 n <= 1392, i16 n <= 9958, i32 n <= 9958, i64 n <= 9958,
 *            i128 n <= 9958, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_prime_factors.h"

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, mid, sqrt = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        __int128 prod = (__int128)mid * mid;
        if (prod == n) {
            sqrt = mid;
            break;
        } else if (prod < n) {
            sqrt = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return sqrt;
}

seq_t a(idx_t n)
{
    /* Return 0 if n is a perfect square. */
    idx_t r = isqrt_u64(n);
    if ((__int128)r * r == n)
        return (seq_t)0;

    /* Ω(n): total number of prime factors with multiplicity. */
    seq_t k = count_prime_factors((seq_t)n);

    /* Return +1 if k is odd, -1 if k is even. */
    return (k & (seq_t)1) ? (seq_t)1 : (seq_t)(-1);
}
