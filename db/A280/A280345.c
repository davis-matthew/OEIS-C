/*
 * A280345  a(0) = 3, a(n+1) = 2*a(n) + periodic sequence of length 2: repeat
 *          [1, -2].
 * https://oeis.org/A280345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(2n)   = 3*4^n
 *          a(2n+1) = 6*4^n + 1
 *
 * Implements the OEIS A280345: a(0)=3, a(n+1)=2*a(n)+[1,-2]_period
 * by exploiting the closed form for even and odd indices.
 */
#include "../utils/pow4.h"

seq_t a(idx_t n) {
    idx_t k = n >> 1;          /* floor(n/2) */
    seq_t pw = pow4(k);        /* 4^k */
    if (n & 1) {               /* odd n = 2k+1 */
        return (seq_t)6 * pw + (seq_t)1;
    } else {                   /* even n = 2k */
        return (seq_t)3 * pw;
    }
}
