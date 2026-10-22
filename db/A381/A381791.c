/*
 * A381791  a(n) = 2*(4 + 17*2^(n-1)).
 * https://oeis.org/A381791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 + 17*2^n
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute 2^n in arbitrary‑precision or large integer form. */
static seq_t pow2_big(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    idx_t e = n;
    while (e)
    {
        if (e & 1UL)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow = pow2_big(n);           /* 2^n */
    return ((seq_t)8) + ((seq_t)17 * pow);
}
