/*
 * A274251  Number of partitions of n^3 into at most three parts.
 * https://oeis.org/A274251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((n^3 + 3)^2 / 12)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is implemented using integer arithmetic: a(n) = ((n^3 + 3)^2 + 6) / 12 */

#include <stdint.h>

/* External linkage function supplied by the build system */
seq_t a(idx_t n)
{
    seq_t nn = n;                    /* cast idx_t to seq_t */
    seq_t n3 = nn * nn * nn;         /* n^3 */

    seq_t v = n3 + 3;                /* n^3 + 3 */
    v *= v;                          /* (n^3 + 3)^2 */

    v += 6;                          /* add rounding offset 6 = 12/2 */
    return v / 12;                   /* integer division gives rounded result */
}
