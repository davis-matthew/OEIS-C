/*
 * A252979  Number of n X 4 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 2 and every value
 *          increasing by 0 or 1 with every step right, diagonally se or down.
 * https://oeis.org/A252979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n > 9 with initial
 *            values: a(1)=3, a(2)=13, a(3)=33, a(4)=68, a(5)=1247, a(6)=8487,
 *            a(7)=27905, a(8)=62977, a(9)=114433 (a(0) is unused, defined as
 *            0 for indexing convenience)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base[10] = {
    0,                /* a(0) unused */
    (seq_t)3,         /* a(1) */
    (seq_t)13,        /* a(2) */
    (seq_t)33,        /* a(3) */
    (seq_t)68,        /* a(4) */
    (seq_t)1247,      /* a(5) */
    (seq_t)8487,      /* a(6) */
    (seq_t)27905,     /* a(7) */
    (seq_t)62977,     /* a(8) */
    (seq_t)114433     /* a(9) */
};

seq_t a(idx_t n)
{
    if (n <= 9) {
        return base[n];
    }

    seq_t a7 = base[7];
    seq_t a8 = base[8];
    seq_t a9 = base[9];

    for (idx_t i = 10; i <= n; ++i) {
        seq_t a10 = ((seq_t)3 * a9) - ((seq_t)3 * a8) + a7;
        a7 = a8;
        a8 = a9;
        a9 = a10;
    }
    return a9;
}
