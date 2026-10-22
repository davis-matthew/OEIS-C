/*
 * A398729  Lower (-1, 1/2) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6), initial values: a(0)=0, a(1)=-1, a(2)=-1, a(3)=1,
 *            a(4)=5, a(5)=12
 * Range:     i8 n <= 9, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Direct access for the first six terms */
    if (n < 6)
    {
        static const seq_t init[6] = {0, -1, -1, 1, 5, 12};
        return init[(size_t)n];
    }

    /* Sliding window of the last six values, initialized with a0..a5 */
    seq_t wnd[6] = {0, -1, -1, 1, 5, 12};

    /* Compute terms from a(6) up to a(n) */
    for (idx_t k = 6; k <= n; ++k)
    {
        seq_t next = 4 * wnd[5]
                   - 7 * wnd[4]
                   + 8 * wnd[3]
                   - 7 * wnd[2]
                   + 4 * wnd[1]
                   -   wnd[0];

        /* Shift window left, drop wnd[0] */
        wnd[0] = wnd[1];
        wnd[1] = wnd[2];
        wnd[2] = wnd[3];
        wnd[3] = wnd[4];
        wnd[4] = wnd[5];
        wnd[5] = next;
    }

    return wnd[5];
}
