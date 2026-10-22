/*
 * A344689  a(n) is the number of preference profiles in the stable marriage
 *          problem with n men and n women such that one man and one woman are
 *          ranked last by all the people of the opposite gender except each
 *          other.
 * https://oeis.org/A344689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4*(n-1)!^(2n) for n ≠ 2; a(2) = 14
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the special case */
    if (n == (idx_t)2)
        return (seq_t)14;

    /* Compute (n-1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)2; i < n; ++i)
        fact = fact * (seq_t)i;

    /* Compute (n-1)!^(2n) */
    seq_t exp = (seq_t)(2 * n);
    seq_t pow = big_pow(fact, exp);

    /* Compute n^4 */
    seq_t term4 = (seq_t)n;
    term4 *= term4;   /* n^2 */
    term4 *= term4;   /* n^4 */

    /* Final result */
    return term4 * pow;
}
