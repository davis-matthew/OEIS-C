/*
 * A390260  Expansion of Sum_{k>=1} (-1)^(k+1)*x^(k^4)/(1-x^(k^4)).
 * https://oeis.org/A390260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Sum_{d^4 | n} (-1)^(d+1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  also a(n) is multiplicative:  a(2^e) = 1 - ⌊e/4⌋,  a(p^e)=1+⌊e/4⌋ (p odd) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {          /* undefined for 0, return 0 for safety */
        return (seq_t)0;
    }

    seq_t result = (seq_t)1;
    uint64_t m = n;

    /* factor 2 separately */
    uint64_t e = 0;
    while ((m & 1ULL) == 0) {
        e++;
        m >>= 1;
    }
    if (e > 0) {
        seq_t contrib = (seq_t)1 - (seq_t)(e / 4);
        result *= contrib;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            seq_t contrib = (seq_t)1 + (seq_t)(e / 4);
            result *= contrib;
        }
    }

    /* any remaining prime factor has exponent 1 */
    if (m > 1) {
        /* exponent 1 => floor(1/4)=0, contribution = 1 + 0 = 1 */
        /* multiplying by 1 has no effect, so we skip it */
    }

    return result;
}
