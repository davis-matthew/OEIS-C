/*
 * A341591  Number of superior prime divisors of n.
 * https://oeis.org/A341591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there exists a prime divisor p of n such that p^2 ≥
 *            n, otherwise a(n) = 0. (a(1) = 0.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Find the largest prime factor of n. */
    idx_t m = n;
    idx_t largest = 0;

    /* Handle factor 2. */
    if ((m & 1) == 0) {
        largest = 2;
        while ((m & 1) == 0)
            m >>= 1;
    }

    /* Check odd factors up to sqrt(m). */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            largest = p;
            while (m % p == 0)
                m /= p;
        }
    }

    /* Remaining m is a prime factor > sqrt(original n). */
    if (m > 1)
        largest = m;

    /* Determine if largest prime factor satisfies p^2 ≥ n. */
    idx_t sq = largest * largest;   /* safe in idx_t */
    return (sq >= n) ? (seq_t)1 : (seq_t)0;
}
