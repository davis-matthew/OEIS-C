/*
 * A254370  a(n) = 35*2^n + 5*4^n + 15*3^n + 5^n + 70.
 * https://oeis.org/A254370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*2^n + 5*4^n + 15*3^n + 5^n + 70
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation: base^exp (exp is idx_t) */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p2 = pow_uint((seq_t)2, n);
    seq_t p3 = pow_uint((seq_t)3, n);
    seq_t p4 = pow_uint((seq_t)4, n);
    seq_t p5 = pow_uint((seq_t)5, n);

    seq_t res = (seq_t)35 * p2
              + (seq_t)5  * p4
              + (seq_t)15 * p3
              + p5
              + (seq_t)70;

    return res;
}
