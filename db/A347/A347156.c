/*
 * A347156  Sum of squares of distinct prime divisors of n that are < sqrt(n).
 * https://oeis.org/A347156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of squares of distinct prime divisors of n that are <
 *            sqrt(n)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes A347156: sum of squares of distinct prime divisors of n that are < sqrt(n) */
seq_t a(idx_t n) {
    if (n <= 1) return 0;
    uint64_t n_orig = (uint64_t)n;
    uint64_t m = n_orig;
    seq_t sum = 0;

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        if ((uint64_t)2 * 2ULL < n_orig) sum += (seq_t)4;
        while ((m & 1ULL) == 0) m >>= 1;
    }

    /* iterate over odd potential factors up to sqrt(n_orig) */
    for (uint64_t p = 3; p <= n_orig / p; p += 2) {
        if (m % p == 0) {
            if (p < n_orig / p) sum += (seq_t)p * (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    return sum;
}
