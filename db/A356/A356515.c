/*
 * A356515  For any n >= 0, let x_n(1) = n, and for any b > 1, x_n(b) is the
 *          sum of digits of x_n(b-1) in base b; x_n is eventually constant,
 *          with value a(n).
 * https://oeis.org/A356515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation follows the definition of OEIS A356515: successively
   sum the digits of the previous value in bases 2, 3, 4, … until the
   value stabilises. */

#include <stdint.h>

/* sum of the digits of `n` written in base `b` */
static inline seq_t digit_sum(seq_t n, seq_t b)
{
    seq_t s = 0;
    while (n != 0)
    {
        s += n % b;
        n /= b;
    }
    return s;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t b = 2;

    while (b <= cur)
    {
        cur = digit_sum(cur, b);
        ++b;
    }
    return cur;
}
