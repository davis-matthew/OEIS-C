/*
 * A277581  Goldbach's problem extended to squares of nonnegative differences
 *          of primes: smallest integer >= ((A112823(n) - A234345(n))^2)/n for
 *          n >= 2.
 * https://oeis.org/A277581
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(((largest prime ≤ n with 2n-p prime)^2)/n)
 * Range:     u8 n <= 21, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;

    idx_t two_n = n << 1;
    idx_t p;
    for (p = n; p >= 2; --p) {
        if (is_prime(p) && is_prime(two_n - p))
            break;                  /* p is the largest prime ≤ n with 2n-p prime */
    }

    idx_t d = two_n - 2 * p;         /* difference q−p where q=2n−p >= p */
    seq_t d_sq = (seq_t)d * (seq_t)d; /* (q−p)^2 */
    return (d_sq + (seq_t)n - 1) / (seq_t)n; /* ceil(d_sq/n) */
}
