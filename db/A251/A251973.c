/*
 * A251973  Number of (n+1)X(7+1) 0..3 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A251973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(ceil((n+1)/2)) * p(floor((n+1)/2)))^2 where p(r) is
 *            the number of r×4 monotone matrices with entries 0..3. This
 *            implementation computes p(r) via dynamic programming over the 35
 *            weakly increasing 4‑tuples of digits 0..3
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define STATE_COUNT 35
#define MAX_R 5001   /* enough for n up to 10000 (rows ≤ 10001) */

/* Weakly increasing 4‑tuples of 0..3.  Generated once. */
static idx_t states[STATE_COUNT][4];

/* For each state j, preds[j] holds the indices of states i with
   states[i][k] ≤ states[j][k] for all 0≤k<4.  */
static idx_t preds[STATE_COUNT][STATE_COUNT];
static int pred_cnt[STATE_COUNT];

/* Memoisation of p(r) for r up to MAX_R. */
static seq_t memo[MAX_R + 1];
static idx_t memo_max = 0;
static bool init_done = false;

/* Helper to initialise states and predicates. */
static void init_states(void)
{
    int sidx = 0;
    /* enumerate all weakly increasing 4‑tuples */
    for (idx_t a = 0; a <= 3; ++a)
        for (idx_t b = a; b <= 3; ++b)
            for (idx_t c = b; c <= 3; ++c)
                for (idx_t d = c; d <= 3; ++d) {
                    states[sidx][0] = a;
                    states[sidx][1] = b;
                    states[sidx][2] = c;
                    states[sidx][3] = d;
                    ++sidx;
                }

    /* compute predecessors for each state */
    for (int j = 0; j < STATE_COUNT; ++j) {
        int cnt = 0;
        for (int i = 0; i < STATE_COUNT; ++i) {
            bool ok = true;
            for (int k = 0; k < 4; ++k)
                if (states[i][k] > states[j][k]) { ok = false; break; }
            if (ok)
                preds[j][cnt++] = i;
        }
        pred_cnt[j] = cnt;
    }

    init_done = true;
}

/* Compute p(r) for r ≥ 0.  r = 0 => 1 (empty matrix). */
static seq_t matrix_count(idx_t r)
{
    if (r == 0) return (seq_t)1;

    if (!init_done) init_states();

    if (r <= memo_max) return memo[r];

    /* dynamic programming arrays */
    static seq_t dp[STATE_COUNT];
    static seq_t newdp[STATE_COUNT];
    static bool dp_initialized = false;

    /* initialise dp for r = 1 if not already done */
    if (!dp_initialized) {
        for (int i = 0; i < STATE_COUNT; ++i)
            dp[i] = (seq_t)1;     /* all one‑row matrices are valid */
        seq_t total = 0;
        for (int i = 0; i < STATE_COUNT; ++i)
            total += dp[i];
        memo[1] = total;
        memo_max = 1;
        dp_initialized = true;
    }

    /* grow DP up to r */
    for (idx_t cur = memo_max + 1; cur <= r; ++cur) {
        for (int j = 0; j < STATE_COUNT; ++j) {
            seq_t sum = 0;
            int pc = pred_cnt[j];
            for (int p = 0; p < pc; ++p)
                sum += dp[preds[j][p]];
            newdp[j] = sum;
        }
        /* copy newdp into dp */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp[i] = newdp[i];

        /* accumulate total for this r */
        seq_t total = 0;
        for (int i = 0; i < STATE_COUNT; ++i)
            total += dp[i];
        memo[cur] = total;
    }

    memo_max = r;
    return memo[r];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* number of rows in the array is n+1 */
    idx_t r_even = (n + 2) / 2;  /* ceil((n+1)/2) */
    idx_t r_odd  = (n + 1) / 2;  /* floor((n+1)/2) */

    /* compute p(r) for the two parities */
    seq_t ce = matrix_count(r_even);   /* p(r_even) */
    seq_t co = matrix_count(r_odd);    /* p(r_odd) */

    /* a(n) = (ce * co)^2 */
    seq_t prod = ce;
    prod *= co;
    prod *= prod;  /* square */

    return prod;
}
