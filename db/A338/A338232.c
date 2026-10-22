/*
 * A338232  Number of ternary strings of length n that contain at least two
 *          0's and at most two 1's.
 * https://oeis.org/A338232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n*2^(n-1) + binomial(n,2)*2^(n-2) - 3*binomial(n,2)
 *            - 3*binomial(n,3) - 2*n - 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t term1 = pow2(n);                     /* 2^n */
    seq_t term2 = ((seq_t)n) * pow2(n - 1);    /* n*2^(n-1) */
    seq_t term3 = binomial(n, 2) * pow2(n - 2);/* C(n,2)*2^(n-2) */
    seq_t term4 = ((seq_t)3) * binomial(n, 2);/* 3*C(n,2) */
    seq_t term5 = ((seq_t)3) * binomial(n, 3);/* 3*C(n,3) */

    seq_t result = term1 + term2 + term3;
    result -= term4;
    result -= term5;
    result -= ((seq_t)2) * n;
    result -= (seq_t)1;

    return result;
}
