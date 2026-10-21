/*
 * A269121  Number of sequences with n copies each of 1,2,3 avoiding the
 *          pattern 123.
 * https://oeis.org/A269121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(3*n) - 3 * Σ_{k=0..n-1} binomial(3*n, k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t m = (idx_t)3 * n;          /* total number of slots: 3n */
    seq_t cur = (seq_t)1;             /* C(m,0) */
    seq_t sum = (seq_t)0;

    /* Sum binomial coefficients C(m,k) for k = 0 .. n-1 */
    for (idx_t k = (idx_t)0; k < n; ++k) {
        sum += cur;
        /* Compute next binomial value multiplicatively */
        cur = (cur * (seq_t)(m - k)) / (seq_t)(k + (idx_t)1);
    }

    seq_t total = pow2(m);            /* 2^(3*n) = 8^n */
    seq_t result = total - (seq_t)3 * sum;
    return result;
}
