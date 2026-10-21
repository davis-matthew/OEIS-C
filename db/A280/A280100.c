/*
 * A280100  a(n) = 4^(2*n) * (n!)^3 * (n+1)!.
 * https://oeis.org/A280100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^n * (n!)^3 * (n+1)! (OEIS A280100)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* a(0) = 1 */

    /* 16^n */
    seq_t pow16 = big_pow((seq_t)16, (seq_t)n);

    /* n! */
    seq_t fact_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* (n + 1)! = n! * (n+1) */
    seq_t fact_n_plus1 = fact_n * ((seq_t)n + (seq_t)1);

    /* (n!)^3 */
    seq_t fact_n3 = fact_n * fact_n * fact_n;

    /* a(n) = 16^n * (n!)^3 * (n+1)! */
    seq_t result = pow16 * fact_n3 * fact_n_plus1;
    return result;
}
