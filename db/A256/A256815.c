/*
 * A256815  Number of length n+7 0..1 arrays with at most two downsteps in
 *          every 7 consecutive neighbor pairs.
 * https://oeis.org/A256815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   OEIS A256815 * Recurrence: a(n) = number of binary arrays of
 *            length n+7 with at most two * 1→0 transitions in every 8
 *            consecutive bits (i.e. 7 consecutive neighbor pairs). * The
 *            implementation uses a linear‐time DP over a 7‑bit state
 *            representing the * last 7 bits of the prefix
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 27, u64 n <= 62, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* length of the binary array */
    idx_t L = n + 7;
    if (L <= 7) {                 /* no 8‑bit window exists */
        return (seq_t)1 << L;    /* all 2^L sequences are valid */
    }

    /* pre‑compute which (state, new_bit) pairs produce a valid 8‑bit window */
    static bool ok[128][2];
    static bool init = false;
    if (!init) {
        for (int s = 0; s < 128; ++s) {
            for (int b = 0; b < 2; ++b) {
                int cnt = 0;
                int prev = (s >> 6) & 1;   /* oldest of the 7 bits */
                for (int i = 5; i >= 0; --i) {
                    int cur = (s >> i) & 1;
                    if (prev == 1 && cur == 0) ++cnt;
                    prev = cur;
                }
                if (prev == 1 && b == 0) ++cnt;   /* last pair with new bit */
                ok[s][b] = (cnt <= 2);
            }
        }
        init = true;
    }

    /* DP arrays: dp[s] = number of valid prefixes of current length ending
     * in state s (the last 7 bits).  ndp is temporary for the next length. */
    seq_t dp[128];
    seq_t ndp[128];
    for (int s = 0; s < 128; ++s) dp[s] = 1;   /* all 7‑bit strings are allowed */

    for (idx_t pos = 8; pos <= L; ++pos) {
        for (int s = 0; s < 128; ++s) ndp[s] = 0;   /* clear next array */
        for (int s = 0; s < 128; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;

            if (ok[s][0]) {
                int ns = ((s << 1) | 0) & 0x7F;  /* shift left, drop oldest bit */
                ndp[ns] += ways;
            }
            if (ok[s][1]) {
                int ns = ((s << 1) | 1) & 0x7F;
                ndp[ns] += ways;
            }
        }
        for (int s = 0; s < 128; ++s) dp[s] = ndp[s];
    }

    /* Sum all possible ending states to get the total number of sequences. */
    seq_t result = 0;
    for (int s = 0; s < 128; ++s) result += dp[s];
    return result;
}
