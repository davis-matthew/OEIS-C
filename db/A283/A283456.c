/*
 * A283456  Row n=4 of A144048.
 * https://oeis.org/A283456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2 + 3*2^n + 2*3^n + 3*4^n)/2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
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
    seq_t two_pow_n  = pow2(n);
    seq_t three_pow_n= pow3(n);
    seq_t four_pow_n = pow4(n);

    seq_t sum = (seq_t)2
            + (seq_t)3 * two_pow_n
            + (seq_t)2 * three_pow_n
            + (seq_t)3 * four_pow_n;

    return sum / (seq_t)2;
}
