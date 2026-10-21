/*
 * A269635  Number of length-n 0..3 arrays with no repeated value differing
 *          from the previous repeated value by other than plus two or minus
 *          1.
 * https://oeis.org/A269635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of length-n 0..3 arrays with no repeated value differing
 *            from the previous repeated value by other than plus two or minus
 *            1. The implementation uses a dynamic‑programming state
 *            consisting of the last element and the most recent value that
 *            has produced a repeat (or none). The recurrence is...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if desired; not strictly required */

#define V 4              /* number of possible values (0..3) */
#define NO_REP 4         /* sentinel for “no previous repeated value” */
#define STATES (V * (V + 1))   /* 4 * 5 = 20 states */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1; undefined otherwise */

    seq_t cur[STATES] = {0};
    seq_t next[STATES];

    /* initialise for length 1: last element = v, no repeat yet */
    for (idx_t v = 0; v < V; ++v)
        cur[v * (V + 1) + NO_REP] = 1;

    if (n == 1) {
        seq_t sum = 0;
        for (int s = 0; s < STATES; ++s)
            sum += cur[s];
        return sum;
    }

    for (idx_t step = 2; step <= n; ++step) {
        /* zero next state */
        for (int s = 0; s < STATES; ++s)
            next[s] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t cnt = cur[s];
            if (cnt == 0)
                continue;

            int last_elem = s / (V + 1);
            int last_rep  = s % (V + 1);

            for (int x = 0; x < V; ++x) {
                if (x == last_elem) {                     /* repeat event */
                    if (last_rep == NO_REP ||
                        (x - last_rep == 2 || x - last_rep == -1)) {
                        int ns = x * (V + 1) + x;          /* new last_rep = x */
                        next[ns] += cnt;
                    }
                } else {                                  /* no repeat */
                    int ns = x * (V + 1) + last_rep;      /* last_rep unchanged */
                    next[ns] += cnt;
                }
            }
        }

        /* swap arrays */
        for (int s = 0; s < STATES; ++s)
            cur[s] = next[s];
    }

    seq_t result = 0;
    for (int s = 0; s < STATES; ++s)
        result += cur[s];
    return result;
}
