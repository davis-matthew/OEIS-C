/*
 * A328088  a(n) = Sum_{k=4..n} ( binomial(n,k)*(k-2)*(2^k-2*k-2) ) -
 *          (2^n-n-1).
 * https://oeis.org/A328088
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1)*(2*n-6) + 2^(n-1)*(-n^2+n+6) - n - 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 4)          /* offset is 4; behaviour below 4 is irrelevant */
        return (seq_t)0;

    seq_t n_val      = (seq_t)n;
    seq_t pow2_n1    = pow2(n - 1);   /* 2^(n-1)   */
    seq_t pow3_n1    = pow3(n - 1);   /* 3^(n-1)   */

    /* term1 = 3^(n-1) * (2*n - 6) */
    seq_t twon_minus6 = (seq_t)(2 * n_val) - (seq_t)6;
    seq_t term1 = pow3_n1 * twon_minus6;

    /* term2 = 2^(n-1) * (n^2 - n - 6)  (note: this is the positive part of (-n^2+n+6)) */
    seq_t n_sq = n_val * n_val;
    seq_t n_sq_minus_n_minus6 = n_sq - n_val - (seq_t)6;
    seq_t term2 = pow2_n1 * n_sq_minus_n_minus6;

    /* final result */
    seq_t result = term1 - term2;
    result -= n_val;
    result -= (seq_t)1;

    return result;
}
