/*
 * A255621  Number of length n+7 0..2 arrays with at most two downsteps in
 *          every 7 consecutive neighbor pairs.
 * https://oeis.org/A255621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 15, u64 n <= 39, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A255621: number of length n+7 arrays over {0,1,2} with at most two
 *  downsteps in every block of 7 consecutive pairs.
 *
 *  We model the constraint by a finite automaton on the last 7 elements.
 *  For each 7‑tuple (state) we pre‑compute
 *      - the number of downsteps among its 6 internal pairs,
 *      - its last element.
 *  When a new digit x (0..2) is appended we form a new window of 7 pairs:
 *      internal_downs + (last > x ? 1 : 0)
 *  and accept it only if this sum ≤ 2.  The new state is obtained by
 *  dropping the oldest digit and shifting left:
 *      new_state = (state % 3⁶) * 3 + x .
 *
 *  We run a DP for n steps, starting from all 3⁷ initial 7‑tuples
 *  (each contributes 1).  After n steps we have processed
 *  7+n = n+7 digits.  The answer is the sum over all final states.
 *
 *  Complexity:   O(n * 3⁷)   (~65·10⁶ operations for n=10 000)
 *  Memory:       O(3⁷)       (two arrays of 2187 entries)
 */

#include <stddef.h>

#define STATES 2187
#define POW729 729   /* 3⁶ */

seq_t a(idx_t n) {
    /* --------- state precomputation (lazy) --------- */
    static unsigned char internal_downs[STATES];
    static unsigned char last_digit[STATES];
    static int ready = 0;
    if (!ready) {
        for (int state = 0; state < STATES; ++state) {
            int tmp = state;
            int d[7];
            for (int k = 6; k >= 0; --k) {
                d[k] = tmp % 3;
                tmp /= 3;
            }
            last_digit[state] = d[6];
            unsigned char cnt = 0;
            for (int i = 0; i < 6; ++i)
                if (d[i] > d[i + 1]) ++cnt;
            internal_downs[state] = cnt;
        }
        ready = 1;
    }

    /* --------- DP arrays --------- */
    static seq_t dp_cur[STATES];
    static seq_t dp_next[STATES];

    /* initialise with all 3⁷ starting 7‑tuples */
    for (int i = 0; i < STATES; ++i)
        dp_cur[i] = 1;

    /* process each additional digit (n steps) */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero next layer */
        for (int i = 0; i < STATES; ++i)
            dp_next[i] = 0;

        for (int state = 0; state < STATES; ++state) {
            seq_t ways = dp_cur[state];
            if (ways == 0) continue;

            unsigned char downs = internal_downs[state];
            unsigned char last  = last_digit[state];

            for (int v = 0; v < 3; ++v) {
                unsigned char new_d = downs + ((last > v) ? 1 : 0);
                if (new_d <= 2) {
                    int new_state = (state % POW729) * 3 + v;
                    dp_next[new_state] += ways;
                }
            }
        }

        /* swap layers */
        for (int i = 0; i < STATES; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum all valid completions */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i)
        total += dp_cur[i];

    return total;
}
