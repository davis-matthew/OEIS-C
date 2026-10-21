/*
 * A254368  a(n) = 5*2^n + 3^n + 15.
 * https://oeis.org/A254368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 5*2^n + 3^n + 15
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p2 = pow2(n);          /* 2^n          */
    seq_t p3 = pow3(n);          /* 3^n          */
    return (seq_t)5 * p2 + p3 + (seq_t)15;
}
