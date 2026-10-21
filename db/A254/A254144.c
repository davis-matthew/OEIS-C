/*
 * A254144  a(n) = 1*6^n + 2*5^n + 3*4^n + 4*3^n + 5*2^n + 6*1^n.
 * https://oeis.org/A254144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1*6^n + 2*5^n + 3*4^n + 4*3^n + 5*2^n + 6*1^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t k;
    for (k = 1; k <= 6; ++k) {
        seq_t base  = (seq_t)(7 - k);      /* 6,5,4,3,2,1 */
        seq_t term  = pow_int(base, n);    /* base^n   */
        result += (seq_t)k * term;         /* k * base^n */
    }
    return result;
}
