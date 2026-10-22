/*
 * A390115  a(n) is the total number of vertices of degree 2 in Catalan word
 *          grid graphs with n parts.
 * https://oeis.org/A390115
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2*binomial(2*n, n) - binomial(2*n+4, n+2)/2 +
 *            3*binomial(2*n+2, n+1) - 6*binomial(2*n-2, n-1)
 * Range:     u8 none, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    idx_t two_n          = n << 1;          /* 2*n          */
    idx_t two_n_plus_2   = two_n + 2;       /* 2*n+2        */
    idx_t two_n_minus_2  = two_n - 2;       /* 2*n-2        */
    idx_t two_n_plus_4   = two_n_plus_2 + 2;/* 2*n+4        */

    seq_t c0 = binomial(two_n,          n);          /* C(2n,   n)   */
    seq_t c1 = binomial(two_n_plus_2,   n + 1);       /* C(2n+2, n+1) */
    seq_t c2 = binomial(two_n_minus_2,  n - 1);       /* C(2n-2, n-1) */
    seq_t c3 = binomial(two_n_plus_4,   n + 2);       /* C(2n+4, n+2) */

    seq_t term1 = c0 * 2;   /* 2*C(2n, n)   */
    seq_t term2 = c3 / 2;   /* C(2n+4, n+2)/2 */
    seq_t term3 = c1 * 3;   /* 3*C(2n+2, n+1) */
    seq_t term4 = c2 * 6;   /* 6*C(2n-2, n-1) */

    seq_t result = term3;                      /* start with positive term */

    /* subtract all negative terms, guarding against underflow */
    if (result >= term1)
        result -= term1;
    else
        return 0;

    if (result >= term2)
        result -= term2;
    else
        return 0;

    if (result >= term4)
        result -= term4;
    else
        return 0;

    return result;
}
