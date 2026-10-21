/*
 * A250332  Number of length n+5 0..4 arrays with every six consecutive terms
 *          having the maximum of some three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 13, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A250332: Count of length n+5 arrays over {0..4} such that every six
 * consecutive terms have the maximum of some three terms equal to the
 * minimum of the remaining three terms. */

#include <stdbool.h>

#define STATE_COUNT 3125
#define POW5_4      625

/* check whether a 6‑element window satisfies the condition */
static bool window_ok(const unsigned char w[6]) {
    for (int i = 0; i < 6; ++i)
        for (int j = i + 1; j < 6; ++j)
            for (int k = j + 1; k < 6; ++k) {
                unsigned char mx = w[i];
                if (w[j] > mx) mx = w[j];
                if (w[k] > mx) mx = w[k];
                unsigned char mn = 5; /* larger than any symbol 0..4 */
                for (int t = 0; t < 6; ++t)
                    if (t != i && t != j && t != k && w[t] < mn)
                        mn = w[t];
                if (mx == mn)
                    return true;
            }
    return false;
}

seq_t a(idx_t n) {
    const idx_t L = n + 5;    /* actual length of the array */

    /* pre‑computed mask: for each 5‑digit suffix the allowed next digits */
    static unsigned char valid_mask[STATE_COUNT];
    static bool initialized = false;

    if (!initialized) {
        /* enumerate all 6‑digit windows */
        for (int a0 = 0; a0 <= 4; ++a0)
            for (int a1 = 0; a1 <= 4; ++a1)
                for (int a2 = 0; a2 <= 4; ++a2)
                    for (int a3 = 0; a3 <= 4; ++a3)
                        for (int a4 = 0; a4 <= 4; ++a4)
                            for (int a5 = 0; a5 <= 4; ++a5) {
                                unsigned char w[6];
                                w[0] = a0; w[1] = a1; w[2] = a2;
                                w[3] = a3; w[4] = a4; w[5] = a5;
                                if (window_ok(w)) {
                                    int code = a0 * POW5_4 + a1 * 125 + a2 * 25 + a3 * 5 + a4;
                                    valid_mask[code] |= (1U << a5);
                                }
                            }
        initialized = true;
    }

    /* DP arrays */
    static seq_t dp[STATE_COUNT];
    static seq_t ndp[STATE_COUNT];

    /* start with all 5‑digit suffixes: one way to end in each suffix */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp[i] = 1;

    /* extend the array one digit at a time */
    for (idx_t step = 0; step < L - 5; ++step) {
        for (int i = 0; i < STATE_COUNT; ++i)
            ndp[i] = 0;

        for (int code = 0; code < STATE_COUNT; ++code) {
            seq_t ways = dp[code];
            if (!ways) continue;
            unsigned char mask = valid_mask[code];
            if (!mask) continue;
            int base = code % POW5_4;    /* drop the most significant digit */
            for (unsigned char a = 0; a < 5; ++a)
                if (mask & (1U << a)) {
                    int next = base * 5 + a;
                    ndp[next] += ways;
                }
        }
        for (int i = 0; i < STATE_COUNT; ++i)
            dp[i] = ndp[i];
    }

    /* sum all suffixes to get the total number of sequences of length L */
    seq_t result = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        result += dp[i];
    return result;
}
