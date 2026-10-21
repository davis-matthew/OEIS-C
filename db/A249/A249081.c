/*
 * A249081  Number of length n+5 0..4 arrays with every six consecutive terms
 *          having two times the sum of some two elements equal to the sum of
 *          the remaining four.
 * https://oeis.org/A249081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 23, u64 n <= 58, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP: a(n) = number of length n+5 arrays over {0,1,2,3,4} where
   each consecutive 6‑block has 2·(sum of two chosen elements) equal to
   the sum of the other four.  The implementation uses dynamic
   programming over the last 5 digits. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define STATES   3125   /* 5^5 possible last‑5‑digit states */
#define DIGITS   5
#define POWER_4  625    /* 5^4, used for state shift */

seq_t a(idx_t n)
{
    /* static data --------------------------------------------------- */
    static seq_t *dp      = NULL;   /* dp[s] = #ways to end with state s */
    static seq_t *dp_next = NULL;
    static seq_t *ans     = NULL;   /* ans[m] = total for length m+5   */
    static size_t ans_cap = 0;
    static idx_t   max_n   = 0;

    static bool ready          = false;
    static bool accept[STATES][DIGITS];
    static size_t dest[STATES][DIGITS];

    /* initialise static tables -------------------------------------- */
    if (!ready) {
        /* allocate DP buffers */
        dp      = (seq_t*)malloc(STATES * sizeof(seq_t));
        dp_next = (seq_t*)malloc(STATES * sizeof(seq_t));
        if (!dp || !dp_next) return 0; /* allocation failure */

        /* all 5‑digit suffixes are initially possible */
        for (size_t s = 0; s < STATES; ++s)
            dp[s] = (seq_t)1;

        /* pre‑compute transitions and acceptance flags */
        for (size_t s = 0; s < STATES; ++s) {
            /* decode the 5 digits of the state */
            int tail[5];
            size_t tmp = s;
            for (int k = 4; k >= 0; --k) {
                tail[k] = (int)(tmp % 5);
                tmp     /= 5;
            }

            for (size_t d = 0; d < DIGITS; ++d) {
                /* build the 6‑digit window */
                int val[6];
                for (int k = 0; k < 5; ++k) val[k] = tail[k];
                val[5] = (int)d;

                /* check the condition for this window */
                int total = 0;
                for (int k = 0; k < 6; ++k) total += val[k];

                bool ok = false;
                if (total % 3 == 0) {
                    int target = total / 3;       /* = val[i]+val[j] */
                    for (int i = 0; i < 6 && !ok; ++i)
                        for (int j = i + 1; j < 6; ++j)
                            if (val[i] + val[j] == target)
                                ok = true;
                }

                accept[s][d] = ok;
                if (ok)
                    dest[s][d] = ((s % POWER_4) * 5) + d;
                else
                    dest[s][d] = 0;   /* unused when not accepted */
            }
        }
        ready = true;
    }

    /* extend computed values up to n ------------------------------ */
    if (n > max_n) {
        /* ensure answer array large enough */
        if (ans_cap == 0)
            ans_cap = 1;
        while (ans_cap <= (size_t)n)
            ans_cap <<= 1;
        ans = (seq_t*)realloc(ans, ans_cap * sizeof(seq_t));
        if (!ans) return 0; /* allocation failure */

        /* iterate through lengths from max_n+1 to n */
        for (idx_t m = max_n + 1; m <= n; ++m) {
            /* zero next buffer */
            for (size_t s = 0; s < STATES; ++s)
                dp_next[s] = (seq_t)0;

            /* perform transitions for one new digit */
            for (size_t s = 0; s < STATES; ++s) {
                seq_t cur = dp[s];
                if (cur == 0)
                    continue;
                for (size_t d = 0; d < DIGITS; ++d) {
                    if (!accept[s][d])
                        continue;
                    size_t s2 = dest[s][d];
                    dp_next[s2] += cur;
                }
            }

            /* swap buffers */
            seq_t *tmp = dp;
            dp = dp_next;
            dp_next = tmp;

            /* sum all counts for current length (which is m+5) */
            seq_t total = (seq_t)0;
            for (size_t s = 0; s < STATES; ++s)
                total += dp[s];
            ans[(size_t)m] = total;
        }

        max_n = n;
    }

    /* return the requested value ------------------------------------ */
    return ans[(size_t)n];
}
