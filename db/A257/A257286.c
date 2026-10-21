/*
 * A257286  a(n) = 5*6^n - 4*5^n.
 * https://oeis.org/A257286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*6^n - 4*5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow6 = pow_seq((seq_t)6, n);
    seq_t pow5 = pow_seq((seq_t)5, n);
    return 5 * pow6 - 4 * pow5;
}
