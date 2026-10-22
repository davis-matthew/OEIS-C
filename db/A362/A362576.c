/*
 * A362576  Number of vertex cuts in the n X n rook complement graph.
 * https://oeis.org/A362576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(2^n - n - 1) + n^2*(2^(n-1)-1)^2 + binomial(n,2)^2
 *            for n≠2, a(2) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)2) return (seq_t)0;

    seq_t nseq      = (seq_t)n;
    seq_t pow2_n    = pow2(n);
    seq_t pow2_n1   = pow2(n-1);

    /* term1 = 2*n*(2^n - n - 1) */
    seq_t term1 = ((seq_t)2 * nseq * (pow2_n - nseq - (seq_t)1));

    /* term2 = n^2 * (2^(n-1)-1)^2 */
    seq_t sub  = pow2_n1 - (seq_t)1;
    seq_t sub2 = sub * sub;
    seq_t term2 = nseq * nseq * sub2;

    /* term3 = binomial(n,2)^2 = (n*(n-1)/2)^2 */
    seq_t bin2 = (nseq * ((seq_t)(n-1))) / ((seq_t)2);
    seq_t term3 = bin2 * bin2;

    return term1 + term2 + term3;
}
