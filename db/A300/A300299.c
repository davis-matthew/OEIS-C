/*
 * A300299  Denominators of r(n) := Sum_{k=0..n-1} 1/Product_{j=0..4} (k + j +
 *          1), for n >= 0, with r(0) = 0.
 * https://oeis.org/A300299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of r(n) where r(n) = n*(50 + 35*n + 10*n^2 +
 *            n^3) / (96 * (1 + n) * (2 + n) * (n + 3) * (4 + n))
 * Range:     u8 none, u16 n <= 2, u32 n <= 179, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;          /* n^2      */
    seq_t n3   = n2 * nseq;            /* n^3      */
    seq_t n4   = n3 * nseq;            /* n^4      */

    /* numerator: n^4 + 10*n^3 + 35*n^2 + 50*n */
    seq_t N = n4 + (seq_t)10 * n3 + (seq_t)35 * n2 + (seq_t)50 * nseq;

    /* denominator: 96 * (n+1)*(n+2)*(n+3)*(n+4) */
    seq_t D = (seq_t)96 * ((nseq + 1) * (nseq + 2) *
                           (nseq + 3) * (nseq + 4));

    seq_t g = gcd(N, D);
    return D / g;
}
