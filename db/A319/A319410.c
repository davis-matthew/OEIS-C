/*
 * A319410  Twice A032741.
 * https://oeis.org/A319410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (d(n) - 1) for n > 0, a(0) = 0 where d(n) is the
 *            number of (positive) divisors of n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute the number of divisors d(n) using prime factorization. */
    idx_t m = n;
    idx_t tau = 1; /* divisor count accumulator */

    /* factor 2 */
    if ((m & 1) == 0) {
        idx_t exp = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++exp;
        }
        tau *= (exp + 1);
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }

    /* if a prime factor > sqrt(n) remains */
    if (m > 1) {
        tau *= 2;
    }

    /* a(n) = 2 * (tau - 1) */
    seq_t res = (seq_t)2 * ((seq_t)tau - (seq_t)1);
    return res;
}
