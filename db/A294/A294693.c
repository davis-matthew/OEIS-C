/*
 * A294693  Number of permutations of [n] avoiding {1243, 2314, 3412}.
 * https://oeis.org/A294693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-n^5 + 5*n^4 - 5*n^3 + 5*(3*2^n+35)*n^2 +
 *            3*(35*2^n+142)*n -120*(7*2^n-8)) / 120 (sample OEIS definition
 *            for N=1243, 2314, 3412-avoiding permutations)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 19, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* cast index to seq_t for arithmetic */
    seq_t s = (seq_t)n;
    seq_t n2 = s * s;                   /* n^2 */
    seq_t n3 = n2 * s;                  /* n^3 */
    seq_t n4 = n3 * s;                  /* n^4 */
    seq_t n5 = n4 * s;                  /* n^5 */
    seq_t t  = pow2(n);                 /* 2^n */

    /* numerator of the expression (exact integer) */
    seq_t N = 0;
    N -= n5;
    N += (seq_t)5 * n4;
    N -= (seq_t)5 * n3;
    N += (seq_t)5 * ( ((seq_t)3 * t + (seq_t)35) * n2 );
    N += (seq_t)3 * ( ((seq_t)35 * t + (seq_t)142) * s );
    N -= (seq_t)120 * ( ((seq_t)7 * t - (seq_t)8) );

    /* divide by 120 to get the final value */
    return N / (seq_t)120;
}
