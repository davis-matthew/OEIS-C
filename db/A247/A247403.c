/*
 * A247403  Number of length n+4 0..7 arrays with no disjoint pairs in any
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 arrays of digits 0..7 with no two
 *            disjoint pairs * in any consecutive five terms having the same
 *            sum
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 25, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed via a sliding‑window DP over the last four digits. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_N 20000       /* enough for n up to about 10000 */
#define STATES 4096       /* 8^4 */
#define DIGITS 8

/* precomputed pair indices and their bit masks */
static const int pair_i[10] = {0,0,0,0,1,1,1,2,2,3};
static const int pair_j[10] = {1,2,3,4,2,3,4,3,4,4};
static const unsigned short pair_mask[10] = {
    (1<<0)|(1<<1), /* (0,1) */
    (1<<0)|(1<<2), /* (0,2) */
    (1<<0)|(1<<3), /* (0,3) */
    (1<<0)|(1<<4), /* (0,4) */
    (1<<1)|(1<<2), /* (1,2) */
    (1<<1)|(1<<3), /* (1,3) */
    (1<<1)|(1<<4), /* (1,4) */
    (1<<2)|(1<<3), /* (2,3) */
    (1<<2)|(1<<4), /* (2,4) */
    (1<<3)|(1<<4)  /* (3,4) */
};

/* Transition table: valid[s][x] == true if appending digit x keeps the
 * window condition for state s (the last four digits). */
static bool valid[STATES][DIGITS];

/* DP arrays */
static seq_t dp_arr1[STATES];
static seq_t dp_arr2[STATES];
static seq_t *dp_prev = dp_arr1;    /* points to array holding counts for current length */
static seq_t *dp_cur  = dp_arr2;    /* used to build next length */

/* Cached aggregated sums a(1), a(2), ... */
static seq_t sum_cache[MAX_N + 1] = {0};

/* Current maximum length (≥4) that has been computed */
static idx_t computed_len = 4;

/* initialization flag */
static bool initialized = false;

/* Initialise transition table and first DP values */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* build transition table */
    for (idx_t s = 0; s < STATES; ++s) {
        int a = (s >> 9) & 7;
        int b = (s >> 6) & 7;
        int c = (s >> 3) & 7;
        int d = s & 7;
        for (int x = 0; x < DIGITS; ++x) {
            int term[5] = {a, b, c, d, x};
            int sum[10];
            for (int k = 0; k < 10; ++k)
                sum[k] = term[pair_i[k]] + term[pair_j[k]];
            bool ok = true;
            for (int i = 0; i < 10 && ok; ++i) {
                for (int j = i + 1; j < 10; ++j) {
                    if ((pair_mask[i] & pair_mask[j]) == 0) { /* disjoint pairs */
                        if (sum[i] == sum[j]) {
                            ok = false;
                            break;
                        }
                    }
                }
            }
            valid[s][x] = ok;
        }
    }

    /* initialise DP for length 4 (any 4 digits are allowed) */
    for (size_t i = 0; i < STATES; ++i)
        dp_prev[i] = (seq_t)1;
    /* dp_cur already zeroed by static initialization */
}

/* Return a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not used, but safe */

    init();

    idx_t target_len = n + 4;   /* length of the array */

    if (target_len > computed_len) {
        for (idx_t len = computed_len + 1; len <= target_len; ++len) {
            /* zero dp_cur array */
            for (size_t i = 0; i < STATES; ++i)
                dp_cur[i] = (seq_t)0;

            /* build next length */
            for (size_t s = 0; s < STATES; ++s) {
                seq_t cnt = dp_prev[s];
                if (cnt == (seq_t)0) continue;
                for (int x = 0; x < DIGITS; ++x) {
                    if (!valid[s][x]) continue;
                    /* next state: shift left 3 base‑8 digits, append x */
                    size_t s_next = ((s & 511) << 3) | (size_t)x; /* (s % 512) * 8 + x */
                    dp_cur[s_next] += cnt;
                }
            }

            /* compute total for this length */
            seq_t total = (seq_t)0;
            for (size_t i = 0; i < STATES; ++i)
                total += dp_cur[i];

            /* cache result: a(n) where n = len - 4 */
            if (len - 4 <= MAX_N)
                sum_cache[len - 4] = total;

            /* prepare for next iteration */
            seq_t *tmp = dp_prev;
            dp_prev = dp_cur;
            dp_cur = tmp;
        }
        computed_len = target_len;
    }

    return sum_cache[n];
}
