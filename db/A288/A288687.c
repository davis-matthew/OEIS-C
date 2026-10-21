/*
 * A288687  Number of n-digit biquanimous strings using digits {0,1,2,3}.
 * https://oeis.org/A288687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (2^(2*n-1) + n - 2^(n-1)*(1+n))  for n > 0
 * a(0) = 1
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* compute 2^(2*n-1) and 2^(n-1) */
    idx_t exp_big = 2*n - 1;
    seq_t term_big = pow2(exp_big);          /* 2^(2*n-1)   */
    seq_t term_small = pow2(n - 1);           /* 2^(n-1)     */

    /* multiply 2^(n-1) by (1+n) */
    seq_t prod = mul_small(term_small, (unsigned int)(n + 1));

    /* result = 2^(2*n-1) + n - prod */
    seq_t result = term_big + (seq_t)n - prod;

    return result;
}
