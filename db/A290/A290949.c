/*
 * A290949  Number of connected dominating sets in the n X n rook complement
 *          graph.
 * https://oeis.org/A290949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n^2) - 2*n*(2^n - 1) + n^2 - 2*n^2*(2^(n-1)-1)^2 +
 *            n^2*(n-1)^2 - 3*binomial(n,2)^2 - 1 (for n>1) a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"          /* pow2(idx_t exp) -> seq_t */
#include "../utils/binomial.h"      /* binomial(idx_t n, idx_t k) -> seq_t */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* convert to seq_t where needed */
    seq_t n_seq   = (seq_t)n;
    seq_t n2_seq  = n_seq * n_seq;

    /* 2^(n^2) */
    seq_t term1 = pow2(n * n);

    /* 2*n*(2^n - 1) */
    seq_t two_n      = (seq_t)2 * n_seq;
    seq_t two_pow_n  = pow2(n);
    seq_t term2      = two_n * (two_pow_n - (seq_t)1);

    /* n^2 itself */
    seq_t term3 = n2_seq;

    /* 2*n^2*(2^(n-1)-1)^2 */
    seq_t two_pow_n_minus1 = pow2(n - 1);
    seq_t diff = two_pow_n_minus1 - (seq_t)1;
    seq_t diff_sq = diff * diff;
    seq_t tmp = (seq_t)2 * n2_seq;
    seq_t term4 = tmp * diff_sq;

    /* n^2*(n-1)^2 */
    seq_t n_minus1   = n - 1;
    seq_t n_minus1_sq = (seq_t)n_minus1 * (seq_t)n_minus1;
    seq_t term5 = n2_seq * n_minus1_sq;

    /* 3*binomial(n,2)^2 */
    seq_t bin2 = binomial(n, 2);
    seq_t bin2_sq = bin2 * bin2;
    seq_t term6 = (seq_t)3 * bin2_sq;

    /* assemble result */
    seq_t ans = term1;
    ans -= term2;
    ans += term3;
    ans -= term4;
    ans += term5;
    ans -= term6;
    ans -= (seq_t)1;

    return ans;
}
