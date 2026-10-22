/*
 * A356840  Largest most common prime factor of n.
 * https://oeis.org/A356840
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = return the largest prime factor of n that has the maximum
 *            exponent in n's prime factorization. If several primes share
 *            that maximal exponent, choose the largest one
 * Range:     u8 n <= 256, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: idx_t is uint64_t, seq_t is an unsigned integer type of arbitrary width. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* out of sequence range */

    uint64_t m = (uint64_t)n;
    uint64_t bestPrime = 0;
    int maxExp = 0;

    /* factor 2 separately for speed */
    int exp = 0;
    while ((m & 1ULL) == 0) {
        ++exp;
        m >>= 1;
    }
    if (exp > 0) {
        if (exp > maxExp) {
            maxExp = exp;
            bestPrime = 2;
        } else if (exp == maxExp && 2 > bestPrime) {
            bestPrime = 2;
        }
    }

    /* odd primes */
    uint64_t p;
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            if (exp > maxExp) {
                maxExp = exp;
                bestPrime = p;
            } else if (exp == maxExp && p > bestPrime) {
                bestPrime = p;
            }
        }
    }

    /* remaining prime factor */
    if (m > 1) {        /* m itself is prime */
        exp = 1;
        if (exp > maxExp) {
            maxExp = exp;
            bestPrime = m;
        } else if (exp == maxExp && m > bestPrime) {
            bestPrime = m;
        }
    }

    return (seq_t)bestPrime;
}
