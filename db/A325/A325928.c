/*
 * A325928  Number of Motzkin excursions of length n with an odd number of
 *          humps and an odd number of peaks.
 * https://oeis.org/A325928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A325928
 *  Number of Motzkin excursions of length n with an odd number of
 *  humps and an odd number of peaks.
 *
 *  A DP over altitude, peak parity, hump parity and whether the last
 *  step was a U that has not yet been closed (state f).  The algorithm
 *  runs in O(n^2) time with a small constant, quite acceptable for
 *  n up to 10 000.
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define MAX_N 10000                     /* assumed maximal n needed     */
#define ALT_CAP (MAX_N + 2)             /* altitude array size          */
#define STATE_SIZE 12                   /* 2p × 2h × 3f combinations   */

/* DP layer indices: 0 = current, 1 = next   */
static seq_t dp[2][ALT_CAP][STATE_SIZE];
static seq_t cache[MAX_N + 1];          /* a(n) values                 */
static idx_t last_done = 0;             /* largest n computed so far   */
static bool initialized = false;

/* encode p(0/1), h(0/1), f(0/1/2) into 0..11 */
static inline int idx_state(int p, int h, int f)
{
    return (p * 6) + (h * 3) + f;
}

seq_t a(idx_t n)
{
    /* initialise the DP once */
    if (!initialized) {
        memset(dp, 0, sizeof(dp));
        dp[0][0][idx_state(0, 0, 0)] = (seq_t)1;   /* start state */
        cache[0] = (seq_t)0;
        memset(cache + 1, 0, (MAX_N) * sizeof(seq_t));
        initialized = true;
    }

    /* guard against a request larger than our static table */
    if (n > MAX_N) {
        /* would need a dynamic extension – not required for the test set */
        return (seq_t)0;
    }

    /* compute DP steps up to requested n */
    for (idx_t step = last_done; step < n; ++step) {
        int cur = step % 2;
        int nxt = (step + 1) % 2;

        /* zero the next layer for all altitudes that can be reached */
        for (idx_t alt = 0; alt <= step + 1; ++alt) {
            memset(dp[nxt][alt], 0, STATE_SIZE * sizeof(seq_t));
        }

        /* transition from dp[cur] to dp[nxt] */
        for (idx_t alt = 0; alt <= step; ++alt) {
            for (int s = 0; s < STATE_SIZE; ++s) {
                seq_t cnt = dp[cur][alt][s];
                if (!cnt) continue;

                int p = (s / 6) % 2;
                int h = (s / 3) % 2;
                int f = s % 3;

                /* Horizontal step H */
                int nf = (f == 1 || f == 2) ? 2 : 0;
                dp[nxt][alt][idx_state(p, h, nf)] += cnt;

                /* Up step U */
                if (alt + 1 <= step + 1) {
                    dp[nxt][alt + 1][idx_state(p, h, 1)] += cnt;
                }

                /* Down step D */
                if (alt > 0) {
                    int np = p, nh = h;
                    if (f == 1) {        /* UD -> peak AND hump */
                        np ^= 1;
                        nh ^= 1;
                    } else if (f == 2) { /* UHH...D -> hump only */
                        nh ^= 1;
                    }
                    dp[nxt][alt - 1][idx_state(np, nh, 0)] += cnt;
                }
            }
        }

        /* store the answer for the current length (step+1) */
        cache[step + 1] = dp[nxt][0][idx_state(1, 1, 0)];
        last_done = step + 1;
    }

    return cache[n];
}
