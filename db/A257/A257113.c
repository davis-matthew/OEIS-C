/*
 * A257113  a(1) = 2, a(2) = 3; thereafter a(n) is the sum of all the previous
 *          terms.
 * https://oeis.org/A257113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(1)=2, a(2)=3, a(3)=5; for n>=4, a(n)=10*2^(n-4)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* handle the first three terms */
    if (n == 1) return 2;
    if (n == 2) return 3;
    if (n == 3) return 5;

    /* n>=4: compute 10 * 2^(n-4) */
    seq_t v = pow2(n - 4);          /* 2^(n-4) */
    return mul_small(v, 10);        /* multiply by 10 */
}
