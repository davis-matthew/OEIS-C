/*
 * A340627  a(n) = (11*2^n - 2*(-1)^n)/3 for n >= 0.
 * https://oeis.org/A340627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*2^n - 2*(-1)^n)/3
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);          /* v = 2^n */
    v *= (seq_t)11;             /* v = 11*2^n */
    if ((n & 1) == 0) {         /* n even: subtract 2   */
        v -= (seq_t)2;
    } else {                    /* n odd:   add  2     */
        v += (seq_t)2;
    }
    return v / (seq_t)3;        /* divide by 3 */
}
