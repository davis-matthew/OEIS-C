/*
 * A265967  Number of nX2 0..4 arrays with the absolute differences of each
 *          element with its with horizontal and vertical neighbors unique.
 * https://oeis.org/A265967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This implementation computes a(n) by dynamic programming over the
   possible rows of a 2‑column array with entries 0..4, keeping track of
   all states that satisfy the uniqueness of horizontal and vertical
   difference constraints.  No closed form is known, so the recursion
   is implemented implicitly through DP.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* State of one row */
static int row_val[25][2];
static int horiz[25];

/* Transition tables */
static bool valid_top[25][25];
static bool valid_mid[25][25][25];

/* DP arrays (current and next)  */
static seq_t dp[25 * 25];
static seq_t dp_next[25 * 25];
static bool dp_ready = false;   /* whether dp holds a valid state for some length */
static size_t dp_rows = 1;      /* number of processed rows with the current dp */

/* Cache of computed a(n) values (index starts at 1) */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* --------------------------------------------------------------------- */
/* initialise all static tables                                         */
static void init_tables(void)
{
    /* all 25 possible rows (values 0..4 in two columns) */
    int idx = 0;
    for (int a = 0; a <= 4; ++a)
        for (int b = 0; b <= 4; ++b) {
            row_val[idx][0] = a;
            row_val[idx][1] = b;
            horiz[idx] = abs(a - b);
            ++idx;
        }

    /* valid_top[p][c] – transition from first row p to second row c
       Checks constraints for the first row cells (horizontal vs.
       vertical down) and for the second row cells (horizontal vs.
       vertical up). */
    for (int p = 0; p < 25; ++p)
        for (int c = 0; c < 25; ++c) {
            bool ok = true;
            int hp = horiz[p];
            int hc = horiz[c];
            int d_left  = abs(row_val[p][0] - row_val[c][0]); /* down diff left */
            int d_right = abs(row_val[p][1] - row_val[c][1]); /* down diff right */
            if (hp == d_left || hp == d_right) ok = false;
            if (hc == d_left || hc == d_right) ok = false;
            valid_top[p][c] = ok;
        }

    /* valid_mid[pp][p][c] – transition from rows pp, p to new row c
       Checks constraints for the middle row p (with up diff from pp
       and down diff to c) and the new row c (horizontal vs. vertical up). */
    for (int pp = 0; pp < 25; ++pp)
        for (int p = 0; p < 25; ++p)
            for (int c = 0; c < 25; ++c) {
                bool ok = true;
                int hp = horiz[p];
                int hc = horiz[c];
                int up0 = abs(row_val[p][0] - row_val[pp][0]);
                int up1 = abs(row_val[p][1] - row_val[pp][1]);
                int dy0 = abs(row_val[p][0] - row_val[c][0]);
                int dy1 = abs(row_val[p][1] - row_val[c][1]);

                /* column 0 of middle row */
                if (hp == up0 || hp == dy0 || up0 == dy0) { ok = false; continue; }
                /* column 1 of middle row */
                if (hp == up1 || hp == dy1 || up1 == dy1) { ok = false; continue; }
                /* new row horizontal vs vertical up */
                if (hc == dy0 || hc == dy1) { ok = false; continue; }

                valid_mid[pp][p][c] = ok;
            }
}

/* Sum all entries of the current dp array */
static seq_t sum_dp(void)
{
    seq_t s = 0;
    for (int i = 0; i < 25 * 25; ++i) s += dp[i];
    return s;
}

/* --------------------------------------------------------------------- */
/* Main sequence routine                                               */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N == 0) return (seq_t)0;   /* not used, but safe */

    /* ensure cache large enough */
    if (cache_len <= N) {
        size_t new_size = N + 1;          /* indices 0..N, we ignore 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        /* initialise new slots to 0 */
        for (size_t i = cache_len; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_len = new_size;
    }

    /* initialise tables once */
    static bool tables_ok = false;
    if (!tables_ok) {
        init_tables();
        tables_ok = true;
    }

    /* a(1) */
    if (cache[1] == 0) cache[1] = (seq_t)25;
    if (N == 1) return cache[1];

    /* initialise dp for two rows if not already done */
    if (!dp_ready || dp_rows < 2) {
        memset(dp, 0, sizeof(dp));
        for (int p = 0; p < 25; ++p)
            for (int c = 0; c < 25; ++c)
                if (valid_top[p][c]) dp[p * 25 + c] = (seq_t)1;
        dp_rows = 2;
        dp_ready = true;
        cache[2] = sum_dp();
    }

    /* extend DP to requested N */
    for (size_t len = dp_rows + 1; len <= N; ++len) {
        /* clear dp_next */
        memset(dp_next, 0, sizeof(dp_next));
        for (int pp = 0; pp < 25; ++pp) {
            for (int p = 0; p < 25; ++p) {
                seq_t val = dp[pp * 25 + p];
                if (!val) continue;
                for (int c = 0; c < 25; ++c) {
                    if (valid_mid[pp][p][c])
                        dp_next[p * 25 + c] += val;
                }
            }
        }
        memcpy(dp, dp_next, sizeof(dp));
        cache[len] = sum_dp();
    }
    dp_rows = N;
    return cache[N];
}
