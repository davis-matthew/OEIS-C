/*
 * A390404  Number of mutual-visibility sets in the n-crown graph.
 * https://oeis.org/A390404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - (n^2 + 3n + 8)*2^(n-2) + (n^4 - 2n^3 + 7n^2 + 6n +
 *            12)/4 for n>=4; a(1)=3, a(2)=7, a(3)=36
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 1) return 3;
    if (n == 2) return 7;
    if (n == 3) return 36;

    seq_t nseq = (seq_t)n;
    seq_t pow4 = pow_int((seq_t)4, n);
    seq_t pow2 = pow_int((seq_t)2, n - 2);

    seq_t n2 = nseq * nseq;
    seq_t n3 = n2 * nseq;
    seq_t n4 = n2 * n2;

    seq_t term2 = ((n2 + (seq_t)3 * nseq + (seq_t)8) * pow2);
    seq_t num = n4 - (seq_t)2 * n3 + (seq_t)7 * n2 + (seq_t)6 * nseq + (seq_t)12;
    seq_t term3 = num / 4;

    return pow4 - term2 + term3;
}
