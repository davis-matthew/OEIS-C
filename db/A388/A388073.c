/*
 * A388073  a(n) = Kronecker symbol (-39/n) = (n/39).
 * https://oeis.org/A388073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Kronecker symbol (-39/n) = (n/39)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Completely multiplicative Dirichlet character modulo 39. */

#include <stdint.h>

/* Mapping of n mod 39 to the value of a(p) for a prime p ≠ 3,13.
   0 means the factor contributes 0 (p = 3 or 13). */
static const int8_t A_MOD[39] = {
    0,  /* 0  */
    1,  /* 1  */
    1,  /* 2  */
    0,  /* 3  */
    1,  /* 4  */
    1,  /* 5  */
    0,  /* 6  */
   -1,  /* 7  */
    1,  /* 8  */
    0,  /* 9  */
    1,  /* 10 */
    1,  /* 11 */
    0,  /* 12 */
    0,  /* 13 */
   -1,  /* 14 */
    0,  /* 15 */
    1,  /* 16 */
   -1,  /* 17 */
    0,  /* 18 */
   -1,  /* 19 */
    1,  /* 20 */
    0,  /* 21 */
    1,  /* 22 */
   -1,  /* 23 */
    0,  /* 24 */
    1,  /* 25 */
    0,  /* 26 */
    0,  /* 27 */
   -1,  /* 28 */
   -1,  /* 29 */
    0,  /* 30 */
   -1,  /* 31 */
    1,  /* 32 */
    0,  /* 33 */
   -1,  /* 34 */
   -1,  /* 35 */
    0,  /* 36 */
   -1,  /* 37 */
   -1   /* 38 */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t res = 1;
    uint64_t m = n;

    /* factorization by simple trial division */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* p = 3 or 13 => zero factor */
            if ((p == 3) || (p == 13))
                return 0;
            if (exp & 1ULL) {          /* odd exponent contributes */
                int8_t val = A_MOD[p % 39];
                /* val is ±1 for primes ≠ 3,13 */
                res *= (seq_t)val;
            }
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        /* m is prime */
        if ((m == 3) || (m == 13))
            return 0;
        /* odd exponent (1) */
        int8_t val = A_MOD[m % 39];
        res *= (seq_t)val;
    }

    return res;
}
