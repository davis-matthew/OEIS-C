/*
 * A385312  a(n) is the number of ternary strings of length n with at least
 *          one 0, at least two 1's and at least three 2's.
 * https://oeis.org/A385312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^(n-2)*(binomial(n,2)+4*n+12) + 3*binomial(n,3) +
 *            4*binomial(n,2) + 4*n + 3 for n >= 4 a(n) = 0 for n < 4
 *            (binomial(n,k) is computed directly for k = 2,3)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* for n < 4 the required counts are 0 */
    if (n < 4)
        return (seq_t)0;

    /* 3^n */
    seq_t pow3n = big_pow((seq_t)3, (seq_t)n);

    /* 2^(n-2)  (n >= 4 so n-2 >= 2) */
    seq_t pow2nm2 = big_pow((seq_t)2, (seq_t)(n - 2));

    /* binomial(n,2) = n*(n-1)/2 */
    seq_t bn2 = (seq_t)n * ((seq_t)n - (seq_t)1) / (seq_t)2;

    /* binomial(n,3) = n*(n-1)*(n-2)/6 */
    seq_t bn3 = (seq_t)n * ((seq_t)n - (seq_t)1) *
                ((seq_t)n - (seq_t)2) / (seq_t)6;

    /* term to be subtracted: 2^(n-2)*(bn2 + 4*n + 12) */
    seq_t sum = bn2 + ((seq_t)4 * (seq_t)n) + (seq_t)12;
    seq_t term_sub = pow2nm2 * sum;

    /* remaining additive terms */
    seq_t term_add = (seq_t)3 * bn3 + (seq_t)4 * bn2 + (seq_t)4 * (seq_t)n
                     + (seq_t)3;

    /* final result */
    return pow3n - term_sub + term_add;
}
