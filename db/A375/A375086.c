/*
 * A375086  Row sums of A375085.
 * https://oeis.org/A375086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n+1) + 4^n + 4*binomial(2*n-2,n-1) -
 *            8*binomial(2*n-2,n-2)*hypergeom([1, 2-n], [1+n], -1))/8 for n >
 *            1. The implementation uses combinatorial simplifications: a(n) =
 *            (2^(n+1) + 4^n)/8 + binomial(2n-2,n-1)/2 - sum_{j=0}^{n-2}
 *            binomial(2n-2,j) (for n≥2). a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offset terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* First part: (2^(n+1) + 4^n) / 8. */
    seq_t pow2_nplus1 = pow2(n + 1);   /* 2^(n+1) */
    seq_t pow4_n      = pow2(2 * n);   /* 4^n = 2^(2n) */
    seq_t part1       = (pow2_nplus1 + pow4_n) / (seq_t)8;

    /* Prepare to accumulate the sum of lower-binomial terms. */
    idx_t two_n_minus_2 = 2 * n - 2;   /* 2n-2, safe for n≥2 */
    seq_t comb   = (seq_t)1;          /* C(2n-2, 0) */
    seq_t sum    = (seq_t)0;          /* Σ_{j=0}^{n-2} C(2n-2, j) */
    for (idx_t j = 0; j <= n - 2; ++j) {
        sum += comb;                  /* add C(2n-2, j) */
        if (j < n - 2) {              /* compute next comb for j+1 */
            /* comb = comb * (2n-2 - j) / (j+1) */
            comb = (comb * (seq_t)(two_n_minus_2 - j)) / (seq_t)(j + 1);
        }
    }

    /* Compute C(2n-2, n-1) from comb = C(2n-2, n-2). */
    seq_t C_nm1 = (comb * (seq_t)n) / (seq_t)(n - 1);
    seq_t part2 = C_nm1 / (seq_t)2;    /* binomial(2n-2,n-1)/2 */

    /* Final result. */
    seq_t result = part1 + part2 - sum;
    return result;
}
