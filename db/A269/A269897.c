/*
 * A269897  Number of n X 3 0..6 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling six exactly
 *          once.
 * https://oeis.org/A269897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 330*a(n-1) - 28773*a(n-2) + 255420*a(n-3) - 599076*a(n-4)
 *            (valid for n > 5, with initial values a(1)=84, a(2)=45360,
 *            a(3)=12377400, a(4)=2801265390, a(5)=579818225700)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const seq_t init[5] = {
    84U,                 /* a(1) */
    45360U,              /* a(2) */
    12377400U,           /* a(3) */
    2801265390U,         /* a(4) */
    579818225700ULL      /* a(5) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* value irrelevant below offset */
    if (n <= 5) return init[n-1];

    seq_t a1 = init[0];
    seq_t a2 = init[1];
    seq_t a3 = init[2];
    seq_t a4 = init[3];
    seq_t a5 = init[4];

    idx_t i;
    for (i = 6; i <= n; ++i) {
        seq_t next = 330 * a5 - 28773 * a4 + 255420 * a3 - 599076 * a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
