/*
 * A320366  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 3
 *          or 4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A320366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 7*a(n-2) + 3*a(n-3) base values: a(1) = 4,
 *            a(2) = 32, a(3) = 228
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle offsets < 1 safely – the offset for this sequence is 1 */
    if (n <= 1) return 4;          /* a(1) */
    if (n == 2) return 32;         /* a(2) */
    if (n == 3) return 228;        /* a(3) */

    /* Iteratively compute the recurrence for n >= 4 */
    seq_t a1 = 4;   /* a(n-3) */
    seq_t a2 = 32;  /* a(n-2) */
    seq_t a3 = 228; /* a(n-1) */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i)
    {
        ai = (seq_t)8 * a3 - (seq_t)7 * a2 + (seq_t)3 * a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
