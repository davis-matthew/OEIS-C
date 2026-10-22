/*
 * A395109  Number of points covered by a regular level n H-Tree on a square
 *          lattice.
 * https://oeis.org/A395109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 3*2^n*(2^n-1)+1, a(2n-1) = binomial(2^(n+1)-1,2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {               /* even n = 2m */
        idx_t m = n >> 1;
        seq_t p   = pow2(m);          /* 2^m */
        seq_t temp = p * (p - 1);     /* 2^m * (2^m - 1) */
        seq_t res  = temp * (seq_t)3 + (seq_t)1;
        return res;
    } else {                           /* odd n = 2m-1, m = (n+1)/2 */
        idx_t m = (n + 1) >> 1;
        seq_t p  = pow2(m + 1);       /* 2^(m+1) */
        seq_t a1 = p - 1;             /* 2^(m+1) - 1 */
        seq_t a2 = p - 2;             /* 2^(m+1) - 2 */
        seq_t res;
        if ((a1 & 1) == 0) {         /* a1 even */
            res = (a1 >> 1) * a2;    /* (a1/2) * a2 */
        } else {                     /* a2 even */
            res = a1 * (a2 >> 1);    /* a1 * (a2/2) */
        }
        return res;
    }
}
