/*
 * A386359  a(n) = (1/4)*(9^n - 2*4^n + 3), n > 0; a(0) = 0.
 * https://oeis.org/A386359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(9^n - 2*4^n + 3), n > 0; a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   mul_small, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    const seq_t nine = (seq_t)9;
    const seq_t four = (seq_t)4;

    seq_t pow9 = pow_int(nine, n);
    seq_t pow4 = pow_int(four, n);

    seq_t two_pow4 = mul_small(pow4, 2);   /* 2 * 4^n */
    seq_t val = pow9;                     /* 9^n */
    val -= two_pow4;                      /* 9^n - 2*4^n */
    val += (seq_t)3;                      /* + 3 */
    return val / 4;                       /* divide by 4 (exact) */
}
