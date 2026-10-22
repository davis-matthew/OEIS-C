/*
 * A356254  Given n balls, all of which are initially in the first of n
 *          numbered boxes, a(n) is the number of steps required to get one
 *          ball in each box when a step consists of moving to the next box
 *          every second ball from the highest-numbered box that has more than
 *          one ball.
 * https://oeis.org/A356254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps required to move one ball into each of
 *            the first n boxes, as defined in A356254. The function simulates
 *            the process using O(n) memory and O(a(n)+n) time
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 1U)           /* a(1) = 0, a(0) is unused */
        return 0;

    /* Allocate one extra element to hold the possible transfer into box n+1 */
    const idx_t arr_size = n + 2;
    seq_t *boxes = (seq_t *)malloc((size_t)arr_size * sizeof(seq_t));
    if (!boxes)
        return 0;         /* out of memory fallback (should not happen for n ≤ 10000) */

    /* initialise all to 0 */
    idx_t i;
    for (i = 0; i < arr_size; ++i)
        boxes[i] = (seq_t)0;

    boxes[1] = (seq_t)n;   /* all balls start in box 1 */

    seq_t steps = 0;
    idx_t cur = 1;         /* highest index ≤ n that currently has >1 ball */

    while (cur <= n && boxes[cur] > (seq_t)1)
    {
        /* number of balls to move from box cur */
        seq_t k = boxes[cur] / 2U;

        /* update current box */
        boxes[cur] -= k;                      /* ceil division */

        /* transfer to the next box */
        boxes[cur + 1U] += k;

        ++steps;

        /* determine the next highest box with >1 ball */
        if ((cur + 1U) <= n && boxes[cur + 1U] > (seq_t)1)
        {
            cur = cur + 1U;                   /* move to the right */
        }
        else
        {
            /* search leftwards for the next box still >1 */
            while (cur > 0 && boxes[cur] <= (seq_t)1)
                --cur;
        }
    }

    free(boxes);
    return steps;
}
