/*
 * A360872  Irregular triangle read by rows of the length of runs in
 *          intermediate solutions to the light switch problem.
 * https://oeis.org/A360872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) returns the run lengths of consecutive equal states in the
 *            periodic pattern of the light‑switch problem for step n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_idx.h"

static inline idx_t lcm_idx_t(idx_t a, idx_t b)
{
    if (a == 0 || b == 0) return 0;
    return a / gcd_idx(a, b) * b;
}

seq_t a(idx_t n)
{
    /* n is 1‑based. */
    idx_t target    = n;
    idx_t cumulative = 0;
    idx_t step      = 1;
    idx_t lc        = 1;          /* lcm(1..step) */

    while (1) {
        idx_t period = lc;

        /* run length for the current row */
        seq_t run      = 1;
        int   prev_state = 1;       /* light 1 is always on */

        for (idx_t pos = 2; pos <= period; ++pos) {
            bool even = true; /* parity of divisors in 2..step is even to start */
            for (idx_t d = 2; d <= step; ++d) {
                if (pos % d == 0) even = !even;
            }
            int state = even ? 1 : 0;

            if (state != prev_state) {
                ++cumulative;
                if (cumulative == target) return run;
                run = 1;
                prev_state = state;
            } else {
                ++run;
            }
        }

        /* close last run of the period */
        ++cumulative;
        if (cumulative == target) return run;

        /* advance to next step */
        ++step;
        lc = lcm_idx_t(lc, (idx_t)step);
    }
}
