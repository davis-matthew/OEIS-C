/*
 * A249842  Number of length n+4 0..6 arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length (n+4) arrays over {0,…,6} such that no
 *            consecutive 5 terms have the maximum of any two equal to the
 *            minimum of the remaining three. Computation: dynamic programming
 *            over 4‑tuples, incrementing the length step by step. The
 *            recurrence is not given in OEIS but is known to be...
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 21, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHA                7               /* size of alphabet */
#define STATES               (ALPHA*ALPHA*ALPHA*ALPHA)   /* 7⁴ = 2401 */
#define WINDOWS              (ALPHA*ALPHA*ALPHA*ALPHA*ALPHA) /* 7⁵ = 16807 */

/* 1 = allowed window, 0 = forbidden */
static uint8_t allowed[WINDOWS];
static bool allowed_inited = false;

/* one‑time construction of the 5‑tuple validity table */
static void init_allowed(void)
{
    for (int code = 0; code < WINDOWS; ++code) {
        int t[5];
        int tmp = code;
        /* decode base‑7 digits: a,b,c,d,x */
        t[4] = tmp % ALPHA; tmp /= ALPHA;
        t[3] = tmp % ALPHA; tmp /= ALPHA;
        t[2] = tmp % ALPHA; tmp /= ALPHA;
        t[1] = tmp % ALPHA; tmp /= ALPHA;
        t[0] = tmp;
        bool forb = false;
        for (int i = 0; i < 5 && !forb; ++i)
            for (int j = i + 1; j < 5 && !forb; ++j) {
                int maxv = t[i] > t[j] ? t[i] : t[j];
                int minR = 10;          /* > 6 */
                for (int k = 0; k < 5; ++k)
                    if (k != i && k != j && t[k] < minR)
                        minR = t[k];
                if (maxv == minR)
                    forb = true;
            }
        allowed[code] = (uint8_t)(!forb);
    }
    allowed_inited = true;
}

/* DP arrays: dp[0] holds the counts for the current length,
   dp[1] a temporary buffer for the next length */
static seq_t dp[2][STATES] = { { 0 } };
static int dp_curr = 0;            /* index of current buffer */
static bool dp_init = false;       /* flag whether dp[0] has been initialized */

/* memoised sequence values a(n), 0‑indexed (a(1) stored at index 1) */
static seq_t *a_values = NULL;
static size_t a_cap = 0;
static idx_t max_n_computed = 0;
static idx_t current_len = 4;      /* length currently represented by dp[dp_curr] */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* not used for OEIS offset */

    /* ensure enough space for memoised results */
    if (!a_values || n + 1 > a_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_values, new_cap * sizeof(seq_t));
        a_values = tmp;
        a_cap = new_cap;
    }

    /* initialise helpers on first call */
    if (!allowed_inited) init_allowed();

    if (!dp_init) {
        /* start with all 4‑tuples counted once each */
        for (size_t s = 0; s < STATES; ++s)
            dp[dp_curr][s] = 1;
        dp_init = true;
        current_len = 4;
        max_n_computed = 0;
    }

    /* extend DP up to requested length n+4 */
    for (idx_t len = current_len + 1; len <= n + 4; ++len) {
        int dp_next = 1 - dp_curr;
        /* clear next buffer */
        memset(dp[dp_next], 0, sizeof(dp[dp_next]));

        /* transition */
        for (size_t s = 0; s < STATES; ++s) {
            seq_t val = dp[dp_curr][s];
            if (!val) continue;
            for (int x = 0; x < ALPHA; ++x) {
                if (allowed[s * ALPHA + x]) {
                    size_t ns = ((s % 343) * ALPHA) + x;   /* 343 = ALPHA³ */
                    dp[dp_next][ns] += val;
                }
            }
        }

        /* total count for this length */
        seq_t total = 0;
        for (size_t s = 0; s < STATES; ++s)
            total += dp[dp_next][s];

        /* store the result for n = len-4 */
        size_t idx = (size_t)(len - 4);
        a_values[idx] = total;
        if (idx > (size_t)max_n_computed)
            max_n_computed = (idx_t)idx;

        /* prepare for next iteration */
        dp_curr = dp_next;
        current_len = len;
    }

    return a_values[n];
}
