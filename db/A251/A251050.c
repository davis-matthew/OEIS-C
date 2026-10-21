/*
 * A251050  Number of (n+1) X (3+1) 0..3 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) – number of (n+1)×4 0..3 arrays with the * forbidden 2×2
 *            block rule. * Computed by dynamic programming over rows, no
 *            explicit recurrence * is used
 * Range:     u8 none, u16 n <= 5, u32 n <= 19, u64 n <= 45, u128 n <= 99,
 *            big any n
 * Helpers:   absdiff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/absdiff.h"

static bool valid[256][256] = {0};
static bool valid_inited   = false;

/* pre–compute the validity of each pair of consecutive rows */
static void init_valid(void)
{
    for (int cur = 0; cur < 256; ++cur) {
        unsigned char c0 = (cur >> 0) & 3;
        unsigned char c1 = (cur >> 2) & 3;
        unsigned char c2 = (cur >> 4) & 3;
        unsigned char c3 = (cur >> 6) & 3;

        for (int prev = 0; prev < 256; ++prev) {
            unsigned char p0 = (prev >> 0) & 3;
            unsigned char p1 = (prev >> 2) & 3;
            unsigned char p2 = (prev >> 4) & 3;
            unsigned char p3 = (prev >> 6) & 3;

            bool ok = true;
            /* column 0–1 */
            if ((int)(c0 + p1) > absdiff((int)c1, (int)p0)) ok = false;
            /* column 1–2 */
            if (ok && (int)(c1 + p2) > absdiff((int)c2, (int)p1)) ok = false;
            /* column 2–3 */
            if (ok && (int)(c2 + p3) > absdiff((int)c3, (int)p2)) ok = false;

            valid[prev][cur] = ok;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* by definition; not used */

    if (!valid_inited) {
        init_valid();
        valid_inited = true;
    }

    /* dp arrays: number of ways to reach each row state */
    seq_t dp[256];
    seq_t next[256];

    /* base: first row can be any state */
    for (int i = 0; i < 256; ++i)
        dp[i] = (seq_t)1;

    /* add rows one by one, total rows = n+1 */
    for (idx_t r = 0; r < n; ++r) {
        for (int i = 0; i < 256; ++i)
            next[i] = (seq_t)0;

        for (int prev = 0; prev < 256; ++prev) {
            seq_t val = dp[prev];
            if (val == (seq_t)0) continue;
            for (int cur = 0; cur < 256; ++cur) {
                if (valid[prev][cur])
                    next[cur] += val;
            }
        }
        /* copy back for next iteration */
        for (int i = 0; i < 256; ++i)
            dp[i] = next[i];
    }

    /* sum all states: total number of arrays with n+1 rows */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 256; ++i)
        result += dp[i];

    return result;
}
