/*
 * A269820  a(n) = 2*(n-1)*a(n-1) - a(n-2) + 2*(n-2)*a(n-3) with
 *          a(0)=a(1)=a(2)=1.
 * https://oeis.org/A269820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n-1)*a(n-1) - a(n-2) + 2*(n-2)*a(n-3), with a(0) =
 *            a(1) = a(2) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n < 3)
        return (seq_t)1;

    /* Keep track of the last three terms: a(i-3), a(i-2), a(i-1). */
    seq_t a_im3 = (seq_t)1;         /* a(0) */
    seq_t a_im2 = (seq_t)1;         /* a(1) */
    seq_t a_im1 = (seq_t)1;         /* a(2) */
    seq_t ai;                      /* a(i) in the loop */

    /* Iterate from i = 3 up to the requested n. */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t term1 = (seq_t)2 * (seq_t)(i - 1) * a_im1;
        seq_t term2 = a_im2;
        seq_t term3 = (seq_t)2 * (seq_t)(i - 2) * a_im3;
        ai = term1 - term2 + term3;

        /* Shift indices for the next iteration. */
        a_im3 = a_im2;
        a_im2 = a_im1;
        a_im1 = ai;
    }

    return a_im1;
}
