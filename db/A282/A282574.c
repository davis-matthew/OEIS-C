/*
 * A282574  The final position on a staircase of n steps during the following
 *          routine: Take steps of length 1 up a staircase until you can't
 *          step any further, then take steps of length 2 down until you can't
 *          step any further, and so on.
 * https://oeis.org/A282574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A282574: the final position on a staircase of *n* steps
   after repeatedly walking up with step size 1, down with size 2,
   up with size 3, and so on until a step size exceeds the number of
   remaining steps.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but guard against 0 */
    idx_t pos = 0;                 /* current position on the staircase */

    for (idx_t step = 1; step <= n; ++step)
    {
        if (step & 1U)             /* odd steps – walk up */
        {
            while (pos + step <= n)
                pos += step;
        }
        else                       /* even steps – walk down */
        {
            while (pos >= step)
                pos -= step;
        }
    }

    return (seq_t)pos;             /* final position */
}
