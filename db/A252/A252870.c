/*
 * A252870  Number of n X 2 nonnegative integer arrays with upper left 0 and
 *          lower right n+2-4 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 10*n^3 - 13*n^2 - 22*n + 24) / 24
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           (OEIS A252870) */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0. */
    if (n < 1)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t n4  = n2 * n2;

    /* Compute numerator as (n^4 + 10*n^3 + 24) - (13*n^2 + 22*n) */
    seq_t pos = n4 + (seq_t)10 * n3 + (seq_t)24;
    seq_t sub = (seq_t)13 * n2 + (seq_t)22 * nn;
    seq_t num = pos - sub;

    return num / (seq_t)24;
}
