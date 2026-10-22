/*
 * A362566  a(n) is the area of the smallest rectangle that the
 *          Harter-Heighway Dragon Curve will fit in after n doublings,
 *          starting with a segment of length 1.
 * https://oeis.org/A362566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*2^n - 5*2^(n/2) + 2)/2 for n even * a(n) = (5*2^n -
 *            9*2^((n-1)/2) + 2)/3 for n ≡ 1 (mod 4) * a(n) = (5*2^n
 *            -13*2^((n-1)/2)+4)/3 for n ≡ 3 (mod 4)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special case n = 0 handled automatically by the even branch */
    if ((n & 1) == 0) {                 /* n is even */
        seq_t two_n     = pow2(n);          /* 2^n   */
        seq_t two_half  = pow2(n >> 1);     /* 2^(n/2) */
        seq_t val = ((seq_t)3) * two_n
                  - ((seq_t)5) * two_half
                  + ((seq_t)2);
        return val / ((seq_t)2);
    }
    else if ((n & 3) == 1) {             /* n ≡ 1 (mod 4) */
        seq_t two_n     = pow2(n);
        seq_t two_half  = pow2((n - 1) >> 1); /* (n-1)/2 */
        seq_t val = ((seq_t)5) * two_n
                  - ((seq_t)9) * two_half
                  + ((seq_t)2);
        return val / ((seq_t)3);
    }
    else {                               /* n ≡ 3 (mod 4) */
        seq_t two_n     = pow2(n);
        seq_t two_half  = pow2((n - 1) >> 1);
        seq_t val = ((seq_t)5) * two_n
                  - ((seq_t)13) * two_half
                  + ((seq_t)4);
        return val / ((seq_t)3);
    }
}
