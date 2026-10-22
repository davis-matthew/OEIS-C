/*
 * A360165  a(n) is the sum of the square roots of the unitary divisors of n
 *          that are odd squares minus the sum of the square roots of the
 *          unitary divisors of n that are even squares.
 * https://oeis.org/A360165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = product over prime powers p^e||n of * 1 + (e even ? (p==2
 *            ? -p^{e/2} : p^{e/2}) : 0)
 * Range:     i8 n <= 8648, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) /* out of scope, but guard for safety */
        return 0;

    seq_t result = 1;
    idx_t m = n;

    while (m > 1) {
        /* Find smallest prime factor p of m */
        idx_t p = 0;
        for (idx_t i = 2; i <= m / i; ++i) {
            if (m % i == 0) { p = i; break; }
        }
        if (p == 0) { /* m is prime */
            p = m;
        }

        /* Count exponent e of p in m */
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* Compute contribution for this prime power */
        seq_t contrib = 1;  /* contribution of divisor 1 */
        if ((e & 1) == 0) { /* e is even */
            /* Compute root = p^{e/2} */
            seq_t root = 1;
            for (idx_t k = 0; k < e / 2; ++k)
                root *= (seq_t)p;

            if (p == 2)
                contrib -= root;   /* even square: subtract */
            else
                contrib += root;   /* odd square: add */
        }

        result *= contrib;
    }

    return result;
}
