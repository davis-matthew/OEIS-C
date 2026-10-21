/*
 * A248720  a(n) = (n*(n+1))^5.
 * https://oeis.org/A248720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+1))^5
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 83, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute t = n * (n + 1) in the register type seq_t */
    seq_t nn = (seq_t)n;
    seq_t t = nn * (nn + 1);

    /* Compute t^5 by repeated multiplication */
    seq_t res = t;
    for (int i = 1; i < 5; ++i)
        res *= t;

    return res;
}
