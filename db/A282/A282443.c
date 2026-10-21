/*
 * A282443  a(n) is the largest step size that is taken on a staircase of n
 *          steps when following the following procedure: Take steps of length
 *          1 up a staircase until you can't step any further, then take steps
 *          of length 2 down until you can't step any further, and so on.
 * https://oeis.org/A282443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: simulate the staircase process to find the largest step size taken. */
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1, but guard against n==0 */

    seq_t pos = 0;
    seq_t maxstep = 0;
    bool up = true;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t step = (seq_t)k;

        if (up) {
            /* Upward step: move as long as pos + step <= n */
            if (pos + step <= (seq_t)n) {
                seq_t t = ((seq_t)n - pos) / step; /* number of forward moves */
                pos += step * t;
                if (t > 0 && step > maxstep)
                    maxstep = step;
            }
        } else {
            /* Downward step: move as long as pos - step >= 0 */
            if (pos >= step) {
                seq_t t = pos / step; /* number of backward moves */
                pos -= step * t;
                if (t > 0 && step > maxstep)
                    maxstep = step;
            }
        }

        up = !up; /* flip direction */
    }

    return maxstep;
}
