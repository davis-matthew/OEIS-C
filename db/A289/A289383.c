/*
 * A289383  Total number of nonzero vectors over all subspaces of an
 *          n-dimensional vector space over the field with two elements.
 * https://oeis.org/A289383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} A022166(n,k)*(2^k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t sum   = (seq_t)0;   /* accumulator for the result          */
    seq_t binom = (seq_t)1;   /* Gaussian binomial coefficient C(n,0)_2 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t pow2k         = pow2(k);           /* 2^k                         */
        seq_t pow2nmk1      = pow2(n - k + 1);   /* 2^(n-k+1)                    */
        seq_t numerator     = pow2nmk1 - (seq_t)1; /* (2^(n-k+1)-1)                */
        seq_t denominator   = pow2k - (seq_t)1;   /* (2^k-1)                      */

        /* Update Gaussian binomial coefficient C(n,k)_2  */
        binom = (binom * numerator) / denominator;

        /* Add the contribution of all non‑zero vectors in
           the k‑dimensional subspaces: C(n,k)_2 * (2^k-1) */
        seq_t term = binom * (pow2k - (seq_t)1);
        sum += term;
    }

    return sum;
}
