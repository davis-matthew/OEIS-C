/*
 * A357511  a(n) = numerator of Sum_{k = 1..n} (1/k) * binomial(n,k)^2 *
 *          binomial(n+k,k)^2 for n >= 1 with a(0) = 0.
 * https://oeis.org/A357511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator of Sum_{k = 1..n} (1/k) * binomial(n, k)^2 *
 *            binomial(n + k, k)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"     /* seq_t gcd(seq_t, seq_t) */
#include "../utils/binomial.h" /* seq_t binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t num = (seq_t)0;  /* current numerator (reduced) */
    seq_t den = (seq_t)1;  /* current denominator (reduced) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial coefficients */
        seq_t cnk  = binomial(n, k);
        seq_t cnkp = binomial(n + k, k);

        /* term numerator = cnk^2 * cnkp^2   */
        seq_t term_num = cnk * cnk;
        term_num = term_num * cnkp * cnkp;

        seq_t term_den = k;

        /* Reduce the single term first */
        seq_t gtd = gcd(term_num, term_den);
        if (gtd != (seq_t)1)
        {
            term_num /= gtd;
            term_den /= gtd;
        }

        /* Add term_num/term_den to the running sum num/den
           using the standard formula with gcd reduction.      */
        seq_t g1  = gcd(den, term_den);
        seq_t d1  = den / g1;
        seq_t d2  = term_den / g1;

        seq_t new_num = num * d2 + term_num * d1;
        seq_t new_den = d1 * term_den;

        seq_t g3 = gcd(new_num, new_den);
        num = new_num / g3;
        den = new_den / g3;
    }

    /* The result is the numerator of the reduced fraction. */
    return num;
}
