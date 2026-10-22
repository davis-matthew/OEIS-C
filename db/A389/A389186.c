/*
 * A389186  Number of mutual-visibility sets in the n-sunlet graph.
 * https://oeis.org/A389186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 4^n + 2*n*(n+3)*2^n + n*(n^2-3*n-25)/3 Formula:
 *            a(2*n+1) = 2*4^n + (2*n+1)*(n+4)*2^n + (2*n+1)*(n^2-5*n-12)/6
 *            The implementation uses the even/odd closed form and handles the
 *            negative intermediate values in the last term with signed
 *            arithmetic
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    if ((n & 1ULL) == 0) {  /* even index: n = 2*m */
        idx_t m   = n >> 1;
        seq_t mseq = (seq_t)m;

        seq_t p2m   = pow2(m);          /* 2^m   */
        seq_t p2_2m = pow2(m << 1);     /* 4^m = 2^(2m) */

        /* term1 = 4^m */
        seq_t term1 = p2_2m;

        /* term2 = 2*m*(m+3)*2^m */
        seq_t term2 = p2m;
        term2 *= (mseq + (seq_t)3);
        term2 *= mseq;
        term2 *= (seq_t)2;

        /* term3 = m*(m^2 - 3*m - 25)/3 */
        __int128 tt1 = (__int128)m * m;                     /* m^2 */
        __int128 tt2 = (__int128)3 * m;                    /* 3*m */
        __int128 tt3 = tt1 - tt2 - 25;                      /* m^2 - 3*m - 25 */
        __int128 term3s = (__int128)m * tt3 / 3;            /* signed quotient */

        result = term1 + term2;
        if (term3s >= 0) {
            result += (seq_t)term3s;
        } else {
            result -= (seq_t)(-term3s);
        }
    } else {            /* odd index: n = 2*m + 1 */
        idx_t m   = (n - 1) >> 1;
        seq_t mseq = (seq_t)m;

        seq_t p2m   = pow2(m);          /* 2^m   */
        seq_t p2_2m = pow2(m << 1);     /* 4^m = 2^(2m) */

        /* term1 = 2*4^m */
        seq_t term1 = p2_2m * (seq_t)2;

        /* term2 = (2*m+1)*(m+4)*2^m */
        seq_t term2 = p2m;
        term2 *= (mseq + (seq_t)4);
        term2 *= ((seq_t)2 * mseq + (seq_t)1);

        /* term3 = (2*m+1)*(m^2 - 5*m - 12)/6 */
        __int128 tt1 = (__int128)m * m;                         /* m^2 */
        __int128 tt2 = (__int128)5 * m;                        /* 5*m */
        __int128 tt3 = tt1 - tt2 - 12;                         /* m^2 - 5*m - 12 */
        __int128 term3s = (__int128)((seq_t)2 * m + 1) * tt3 / 6;  /* signed quotient */

        result = term1 + term2;
        if (term3s >= 0) {
            result += (seq_t)term3s;
        } else {
            result -= (seq_t)(-term3s);
        }
    }

    return result;
}
