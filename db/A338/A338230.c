/*
 * A338230  Number of ternary strings of length n that contain at least two
 *          0's and at most one 1.
 * https://oeis.org/A338230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n*2^(n-1) - 2*binomial(n,2) - 2*n - 1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t pow_n         = pow2(n);          /* 2^n   */
    seq_t pow_n_minus1  = pow2(n - 1);      /* 2^(n-1) */
    seq_t term1 = pow_n;
    seq_t term2 = (seq_t)n * pow_n_minus1; /* n*2^(n-1) */
    seq_t term3 = binomial(n, 2);          /* binomial(n,2) */

    seq_t res = term1 + term2;            /* 2^n + n*2^(n-1) */
    res -= (seq_t)2 * term3;              /* - 2*binomial(n,2) */
    res -= (seq_t)2 * n;                  /* - 2*n */
    res -= (seq_t)1;                      /* - 1 */

    return res;
}
