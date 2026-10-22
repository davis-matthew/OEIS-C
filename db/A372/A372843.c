/*
 * A372843  a(n) is the number of parking functions of order n for which the
 *          third spot is lucky.
 * https://oeis.org/A372843
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2/3)*(n+1)^(n-1) - (1/3)*(2*n-1)*(n-2)^(n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* undefined for n < 3, return 0 (behavior below offset doesn't matter) */
    if (n < 3) return 0;

    /* Compute the two powers */
    seq_t pow1 = big_pow((seq_t)n + 1, (seq_t)n - 1);      /* (n+1)^(n-1) */
    seq_t pow2 = big_pow((seq_t)n - 2, (seq_t)n - 2);      /* (n-2)^(n-2) */

    /* (2*n-1) as seq_t */
    seq_t two_n_minus_one = (seq_t)(2 * n - 1);

    /* Numerator: 2*pow1 - (2*n-1)*pow2 */
    seq_t num = pow1 * 2 - two_n_minus_one * pow2;

    /* Division by 3 (exact) */
    return num / 3;
}
