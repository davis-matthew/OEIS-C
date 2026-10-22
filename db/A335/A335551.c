/*
 * A335551  Number of words of length n over the alphabet {0,1,2} that contain
 *          the substring 12 but not the substring 01.
 * https://oeis.org/A335551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A335551: Number of words of length n over {0,1,2} that contain the substring 12 but not 01.
   Implemented by a simple DP on the last character and whether a 12 has already occurred. */
#include <stddef.h>

seq_t a(idx_t n) {
    /* For n<2 there is no 12 to contain. */
    if (n < 2) return 0;

    /* dp[last][seen12] holds counts for the current length.
       last: 0 = '0', 1 = '1', 2 = '2'.
       seen12: 0 = not yet seen, 1 = already seen. */
    seq_t dp[3][2] = {{0}};
    seq_t ndp[3][2] = {{0}};

    /* Length 1 initialization: no 12 yet. */
    dp[0][0] = dp[1][0] = dp[2][0] = 1;

    for (idx_t len = 1; len < n; ++len) {
        /* Clear ndp for the next length. */
        ndp[0][0] = ndp[0][1] = ndp[1][0] = ndp[1][1] =
        ndp[2][0] = ndp[2][1] = 0;

        for (int c = 0; c < 3; ++c)          /* previous character */
            for (int s = 0; s < 2; ++s) {    /* 12 seen flag */
                seq_t val = dp[c][s];
                if (!val) continue;
                for (int d = 0; d < 3; ++d) { /* new character */
                    if (c == 0 && d == 1) continue;     /* forbid substring 01 */
                    int ns = s;
                    if (!s && c == 1 && d == 2) ns = 1;  /* detect 12 */
                    ndp[d][ns] += val;
                }
            }

        /* Move to next state. */
        dp[0][0] = ndp[0][0]; dp[0][1] = ndp[0][1];
        dp[1][0] = ndp[1][0]; dp[1][1] = ndp[1][1];
        dp[2][0] = ndp[2][0]; dp[2][1] = ndp[2][1];
    }

    /* Sum over all endings with at least one '12'. */
    seq_t result = 0;
    for (int c = 0; c < 3; ++c)
        result += dp[c][1];

    return result;
}
