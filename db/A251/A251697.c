/*
 * A251697  a(n) = (5*n+1) * (6*n+1)^(n-2) * 7^n.
 * https://oeis.org/A251697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n+1) * (6*n+1)^(n-2) * 7^n Special cases: a(0)=1,
 *            a(1)=6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* n >= 2 */
    seq_t factor1 = (seq_t)(5 * n + 1);
    seq_t base2   = (seq_t)(6 * n + 1);
    seq_t exp2    = (seq_t)(n - 2);
    seq_t base7   = (seq_t)7;
    seq_t exp7    = (seq_t)n;

    seq_t pow2 = big_pow(base2, exp2);
    seq_t pow7 = big_pow(base7, exp7);

    return factor1 * pow2 * pow7;
}
