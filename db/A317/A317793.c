/*
 * A317793  a(n) = (4^n + (-3)^n + 2^n + (-1)^n)/2.
 * https://oeis.org/A317793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n + (-3)^n + 2^n + (-1)^n)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0’ */
    seq_t p2 = pow2(n);                    /* 2^n  */
    seq_t p3 = pow3(n);                    /* 3^n  */
    seq_t p4 = pow4(n);                    /* 4^n  */
    seq_t sum;

    if ((n & 1) == 0) {                    /* n even: all terms positive */
        sum = p4 + p3 + p2 + ((seq_t)1);
    } else {                               /* n odd: (-3)^n and (-1)^n are negative */
        sum = p4 + p2;
        sum -= p3;
        sum -= ((seq_t)1);
    }
    return sum >> 1;                       /* divide by 2 */
}
