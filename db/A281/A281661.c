/*
 * A281661  The least common multiple of 1 + n^2 and 1 + n^3.
 * https://oeis.org/A281661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(1 + n^2, 1 + n^3) Equivalent: a(n) = ((3 + (-1)^n)*(1
 *            + n^2 + n^3 + n^5)) / 4 Simplified: if n is odd, a(n) = (1 + n^2
 *            + n^3 + n^5)/2; else a(n) = 1 + n^2 + n^3 + n^5
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;        /* n^2 */
    seq_t n3 = n2 * ns;        /* n^3 */
    seq_t n5 = n2 * n3;        /* n^5 */
    seq_t sum = 1 + n2 + n3 + n5;  /* 1 + n^2 + n^3 + n^5 */
    if (n & 1)                 /* n odd → divide sum by 2 */
        return sum >> 1;
    else                       /* n even → return sum */
        return sum;
}
