/*
 * A348541  Number of partitions of n into 3 parts (r,s,t) such that n | (r^2
 *          + s^2 + t^2).
 * https://oeis.org/A348541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of n into 3 parts (r, s,
 *            t) such that n divides r² + s² + t²
 * Range:     u8 n <= 20, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t count = 0;
    seq_t N = n;                /* cast to seq_t once for modulo */

    for (idx_t r = 1; r <= n / 3; ++r) {
        seq_t r_sq = (seq_t)r * r;
        /* s ranges from r to floor((n - r)/2) ensuring s ≤ t */
        for (idx_t s = r; s <= (n - r) / 2; ++s) {
            idx_t t = n - r - s;
            seq_t sumsq = r_sq + (seq_t)s * s + (seq_t)t * t;
            if (sumsq % N == 0) {
                ++count;
            }
        }
    }
    return count;
}
