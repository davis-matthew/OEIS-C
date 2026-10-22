/*
 * A390340  Expansion of e.g.f. exp(3*x)*(exp(2*x) - x^2 - x).
 * https://oeis.org/A390340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - n*(n+2)*3^(n-2) Valid for n >= 0 with a(0)=1,
 *            a(1)=4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h" /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;

    /* compute 5^n */
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);

    /* compute 3^(n-2) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 2));

    /* compute n*(n+2) */
    seq_t mult = (seq_t)n * ((seq_t)n + 2);

    /* compute n*(n+2)*3^(n-2) */
    seq_t sub = mult * pow3;

    /* result */
    return pow5 - sub;
}
