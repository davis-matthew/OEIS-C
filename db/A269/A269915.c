/*
 * A269915  Number of ternary strings of length n with maximal run length
 *          three containing 1112.
 * https://oeis.org/A269915
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Counts ternary strings of length n with no run longer than 3 that contain the substring 1112.
   Uses dynamic programming over last symbol, run length (1..3), and a 5‑state KMP automaton for "1112".
   a(4)=1, a(5)=5, a(6)=21, … */

#include <stdlib.h>

#define STATES 45  /* 3 symbols * 3 run lengths * 5 automaton states */

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* offset is 4 */

    static const int nxt[5][3] = {
        /* state 0 */
        { 0, 1, 0 },
        /* state 1 */
        { 0, 2, 0 },
        /* state 2 */
        { 0, 3, 0 },
        /* state 3 */
        { 0, 3, 4 },
        /* state 4 (matched) */
        { 4, 4, 4 }
    };

    seq_t dp[STATES]     = { 0 };
    seq_t nextdp[STATES];

    /* initial position: length 1 strings */
    for (int sym = 0; sym < 3; ++sym) {
        int as = nxt[0][sym];
        int id = sym * 15 + (1 - 1) * 5 + as;  /* run_len = 1 ⇒ (1-1) */
        dp[id] += 1;
    }

    /* iterate over remaining positions */
    for (int pos = 2; pos <= n; ++pos) {
        for (int i = 0; i < STATES; ++i) nextdp[i] = 0;

        for (int id = 0; id < STATES; ++id) {
            seq_t ways = dp[id];
            if (!ways) continue;

            int run_sym = id / 15;
            int rem     = id % 15;
            int run_len = rem / 5 + 1;
            int as      = rem % 5;

            for (int sym = 0; sym < 3; ++sym) {
                int new_run_len = (sym == run_sym) ? run_len + 1 : 1;
                if (new_run_len > 3) continue;          /* run length limit */

                int new_as = nxt[as][sym];
                int new_id = sym * 15 + (new_run_len - 1) * 5 + new_as;
                nextdp[new_id] += ways;
            }
        }

        /* copy nextdp into dp for next iteration */
        for (int i = 0; i < STATES; ++i) dp[i] = nextdp[i];
    }

    /* sum states where substring 1112 has been seen (automaton state 4) */
    seq_t ans = 0;
    for (int id = 0; id < STATES; ++id)
        if (id % 5 == 4) ans += dp[id];

    return ans;
}
