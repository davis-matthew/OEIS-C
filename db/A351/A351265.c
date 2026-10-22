/*
 * A351265  Sum of the squares of the squarefree divisors of n.
 * https://oeis.org/A351265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, changed
 * Formula:   a(n) = Π_{p|n} (1 + p^2) (product over distinct primes dividing
 *            n)
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t tmp = n;

    /* factor tmp by trial division, ignoring multiplicities */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            /* p is a distinct prime divisor */
            seq_t p2 = (seq_t)p * (seq_t)p;
            result *= (1 + p2);

            while (tmp % p == 0) {
                tmp /= p;          /* remove all powers of p */
            }
        }
    }

    /* if a prime factor > sqrt(tmp) remains */
    if (tmp > 1) {
        seq_t p2 = (seq_t)tmp * (seq_t)tmp;
        result *= (1 + p2);
    }

    return result;
}
