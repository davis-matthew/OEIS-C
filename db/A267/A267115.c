/*
 * A267115  Bitwise-AND of the exponents of primes in the prime factorization
 *          of n, a(1) = 0.
 * https://oeis.org/A267115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bitwise AND of the exponents of the prime factors of n,
 *            a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return the value of the OEIS sequence at index n. */
seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n == 1u)
        return (seq_t)0;

    seq_t res = 0;          /* accumulator for the bitwise AND */
    bool first = true;      /* flag for the first exponent encountered */

    /* Factorization of n */
    idx_t m = n;

    /* Handle factor 2 separately for speed */
    if ((m & 1u) == 0) {
        seq_t e = 0;
        while ((m % 2u) == 0u) {
            m /= 2u;
            ++e;
        }
        if (first) {
            res = e;
            first = false;
        } else {
            res &= e;
        }
    }

    /* Loop over odd candidates */
    for (idx_t p = 3u; p * p <= m; p += 2u) {
        if (m % p == 0u) {
            seq_t e = 0;
            while (m % p == 0u) {
                m /= p;
                ++e;
            }
            if (first) {
                res = e;
                first = false;
            } else {
                res &= e;
            }
        }
    }

    /* If remainder m is > 1, it is a prime factor with exponent 1 */
    if (m > 1u) {
        if (first) {
            res = (seq_t)1;
            first = false;
        } else {
            res &= (seq_t)1;
        }
    }

    return res;
}
