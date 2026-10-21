/*
 * A273653  Number of forests of labeled rooted trees of height at most 1,
 *          with n labels, three of which are used for root nodes and any root
 *          may contain >= 1 labels.
 * https://oeis.org/A273653
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,3) * (1 + 3*2^(n-3) + 3^(n-3))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    /* Compute binomial coefficient C(n,3) = n*(n-1)*(n-2)/6 */
    seq_t comb = (seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2);
    comb /= (seq_t)6;

    /* Compute 2^(n-3) and 3^(n-3) */
    idx_t exp = n - 3;
    seq_t pow2 = big_pow((seq_t)2, (seq_t)exp);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)exp);

    /* Sum = 1 + 3*2^(n-3) + 3^(n-3) */
    seq_t sum = pow3;
    sum += pow2 * (seq_t)3;
    sum += (seq_t)1;

    /* Result = C(n,3) * sum */
    return comb * sum;
}
