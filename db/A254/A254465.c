/*
 * A254465  a(n) = 35*2^n + 10*4^n + 20*3^n + 4*5^n + 6^n + 56.
 * https://oeis.org/A254465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*2^n + 10*4^n + 20*3^n + 4*5^n + 6^n + 56
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp where base and result are seq_t and exp is idx_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p2 = pow_seq((seq_t)2, n);   /* 2^n */
    seq_t p3 = pow_seq((seq_t)3, n);   /* 3^n */
    seq_t p4 = pow_seq((seq_t)4, n);   /* 4^n */
    seq_t p5 = pow_seq((seq_t)5, n);   /* 5^n */
    seq_t p6 = pow_seq((seq_t)6, n);   /* 6^n */

    seq_t result = 35 * p2 + 10 * p4 + 20 * p3 + 4 * p5 + p6 + 56;
    return result;
}
