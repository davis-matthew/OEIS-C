/*
 * A273654  Number of forests of labeled rooted trees of height at most 1,
 *          with n labels, four of which are used for root nodes and any root
 *          may contain >= 1 labels.
 * https://oeis.org/A273654
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,4) * ( 1 + 7*2^(n-4) + 6*3^(n-4) + 4^(n-4) )
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;                /* term undefined before offset */

    /* compute j^(n-4) for j = 2,3,4 */
    seq_t exp = (seq_t)(n - 4);
    seq_t pow2 = big_pow((seq_t)2, exp);
    seq_t pow3 = big_pow((seq_t)3, exp);
    seq_t pow4 = big_pow((seq_t)4, exp);

    /* sum = 1 + 7*2^(n-4) + 6*3^(n-4) + 4^(n-4) */
    seq_t sum = (seq_t)1;
    sum += pow2 * (seq_t)7;
    sum += pow3 * (seq_t)6;
    sum += pow4;

    /* binomial coefficient C(n,4) = n*(n-1)*(n-2)*(n-3)/24  */
    seq_t bin = (seq_t)n;
    bin *= (seq_t)(n - 1);
    bin *= (seq_t)(n - 2);
    bin *= (seq_t)(n - 3);
    bin /= (seq_t)24;   /* 4! = 24 */

    return bin * sum;
}
