/*
 * A282573  The number of steps taken on a staircase of n steps during the
 *          following routine: Take steps of length 1 up a staircase until you
 *          can't step any further, then take steps of length 2 down until you
 *          can't step any further, and so on.
 * https://oeis.org/A282573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of steps performed by the alternating
 *            staircase routine (up with step 1, down with step 2, up with
 *            step 3, …, stopping when no step of the current size can be
 *            taken)
 * Range:     u8 n <= 63, u16 n <= 7483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t steps = 0;      /* total steps taken */
    seq_t pos   = 0;      /* current position on the staircase */

    for (idx_t k = 1; ; ++k) {
        seq_t step = (seq_t)k;

        if (k & 1) {            /* odd step size: move up */
            seq_t t = ((seq_t)n - pos) / step;
            if (t == 0) break;   /* cannot move even once */
            steps += t;
            pos   += t * step;
        } else {                /* even step size: move down */
            seq_t t = pos / step;
            if (t == 0) break;   /* cannot move even once */
            steps += t;
            pos   -= t * step;
        }
    }

    return steps;
}
