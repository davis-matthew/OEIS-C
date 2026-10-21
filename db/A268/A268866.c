/*
 * A268866  Records in A268865.
 * https://oeis.org/A268866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n even: (4^(n+1)+2)/3 ; n odd: (7*4^(n-1)+2)/3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                /* even n */
        seq_t v = pow4(n + 1);         /* 4^(n+1) */
        v += (seq_t)2;
        v /= (seq_t)3;
        return v;
    } else {                            /* odd n */
        seq_t v = pow4(n - 1);         /* 4^(n-1) */
        v *= (seq_t)7;
        v += (seq_t)2;
        v /= (seq_t)3;
        return v;
    }
}
