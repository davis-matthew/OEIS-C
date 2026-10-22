/*
 * A344747  a(n) = (1/6)*(3^n + (-2)^n - 1).
 * https://oeis.org/A344747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(3^n + (-2)^n - 1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t pow3 = pow_seq((seq_t)3, (int)n);
    seq_t pow2 = pow_seq((seq_t)2, (int)n);

    seq_t sum;
    if ((n & 1) == 0) {
        sum = pow3 + pow2;   /* (-2)^n > 0 for even n */
    } else {
        sum = pow3 - pow2;   /* (-2)^n < 0 for odd n */
    }
    sum -= 1;
    sum /= 6;                 /* exact division by 6 */
    return sum;
}
