/*
 * A385589  a(n) = 2^(n-2)*(3*binomial(n,3) + 6*binomial(n,2) + 6*n + 4).
 * https://oeis.org/A385589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) * (3*binomial(n,3) + 6*binomial(n,2) + 6*n + 4)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           (Adapted for n=0 and n=1 by dividing by 4 and 2 respectively) */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;

    /* Compute binomial coefficients (avoid division before multiplication). */
    seq_t c3 = 0;           /* binomial(n,3) */
    seq_t c2 = 0;           /* binomial(n,2) */

    if (n >= 3) {
        /* n*(n-1)*(n-2)/6  -- all operations are on seq_t */
        c3 = nseq * (nseq - 1) * (nseq - 2) / 6;
    }
    if (n >= 2) {
        /* n*(n-1)/2 */
        c2 = nseq * (nseq - 1) / 2;
    }

    /* Polynomial part: 3*c3 + 6*c2 + 6*n + 4 */
    seq_t poly = 3 * c3 + 6 * c2 + 6 * nseq + 4;

    /* Handle n < 2 by dividing, otherwise multiply by 2^(n-2). */
    if (n == 0) {
        return poly / 4;   /* 2^( -2 ) = 1/4 */
    }
    if (n == 1) {
        return poly / 2;   /* 2^( -1 ) = 1/2 */
    }

    /* n >= 2 : multiply by 2^(n-2) */
    seq_t p2 = pow2(n - 2);          /* returns 2^(n-2) as seq_t */
    return poly * p2;
}
