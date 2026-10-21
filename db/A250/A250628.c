/*
 * A250628  Number of (n+1)X(4+1) 0..2 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of (n+1)×5 arrays with entries 0..2 satisfying 1.
 *            For each row i and each j=2..5, x(i,j)+x(i,j-1) <=
 *            x(i+1,j)+x(i+1,j-1) 2. For each column pair j-1,j and each row
 *            i>1, min(x(i-1,j-1),x(i,j-1)) <= min(x(i-1,j),x(i,j)) The
 *            function uses dynamic programming over the 3^5=243 possible
 *            ro...
 * Range:     u8 none, u16 n <= 3, u32 n <= 15, u64 n <= 109, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- Precomputed tables ---------- */
#define ROWS 243          /* 3^5 possible rows */
#define MAX_NEIGH 243     /* maximum neighbours per row */

/* Row contents and adjacent sums.  */
static int row_vals[ROWS][5];
static int row_sums[ROWS][4];
/* Transition lists.  */
static int neighbor_cnt[ROWS];
static int neighbors[ROWS][MAX_NEIGH];
static bool tables_inited = false;

/* Initialisation of row data and transition lists.  */
static void init_tables(void)
{
    if (tables_inited) return;
    tables_inited = true;

    /* Enumerate all rows as base‑3 numbers. */
    for (int idx = 0; idx < ROWS; ++idx) {
        int x = idx;
        for (int j = 0; j < 5; ++j) {
            row_vals[idx][j] = x % 3;
            x /= 3;
        }
    }
    /* Compute adjacent column sums for each row.  */
    for (int r = 0; r < ROWS; ++r) {
        for (int j = 0; j < 4; ++j)
            row_sums[r][j] = row_vals[r][j] + row_vals[r][j + 1];
    }
    /* Build adjacency lists according to the two constraints.  */
    for (int p = 0; p < ROWS; ++p) {
        neighbor_cnt[p] = 0;
        for (int c = 0; c < ROWS; ++c) {
            /* 1. nondecreasing horizontal sums. */
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                if (row_sums[p][j] > row_sums[c][j]) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;

            /* 2. nondecreasing column minima. */
            int min_val = (row_vals[p][0] < row_vals[c][0]) ?
                           row_vals[p][0] : row_vals[c][0];
            for (int j = 1; j < 5; ++j) {
                int m = (row_vals[p][j] < row_vals[c][j]) ?
                           row_vals[p][j] : row_vals[c][j];
                if (m < min_val) {
                    ok = false;
                    break;
                }
                min_val = m;
            }
            if (!ok) continue;

            neighbors[p][neighbor_cnt[p]++] = c;
        }
    }
}

/* ---------- Cached sequence values ---------- */
static seq_t *cache = NULL;      /* cache[0] = a(1), cache[1] = a(2), ... */
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* DP arrays for the current and next heights.  */
static seq_t dp_curr[ROWS];
static seq_t dp_next[ROWS];

/* Ensure the cache contains values up to a(n).  */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_len) return;
    size_t need = (size_t)n;  /* number of terms needed */

    /* Grow cache storage.  */
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise tables once.  */
    init_tables();

    /* If no terms have been computed yet, initialise DP for height=2.  */
    if (cache_len == 0) {
        /* Height = 1 (single row) : one way for each row.  */
        for (int r = 0; r < ROWS; ++r) dp_curr[r] = 1;

        /* Transition to height = 2.  */
        memset(dp_next, 0, sizeof(dp_next));
        for (int p = 0; p < ROWS; ++p) {
            seq_t val = dp_curr[p];
            if (!val) continue;
            int cnt = neighbor_cnt[p];
            for (int k = 0; k < cnt; ++k) {
                int nxt = neighbors[p][k];
                dp_next[nxt] += val;
            }
        }
        seq_t tot = 0;
        for (int r = 0; r < ROWS; ++r) tot += dp_next[r];

        cache[0] = tot;
        cache_len = 1;
        memcpy(dp_curr, dp_next, sizeof(dp_curr));
    }

    /* Extend the cached sequence up to n.  */
    for (size_t idx = cache_len; idx < need; ++idx) {
        /* dp_curr corresponds to the current height (idx+2).  */
        /* Compute counts for the next height.  */
        memset(dp_next, 0, sizeof(dp_next));
        for (int p = 0; p < ROWS; ++p) {
            seq_t val = dp_curr[p];
            if (!val) continue;
            int cnt = neighbor_cnt[p];
            for (int k = 0; k < cnt; ++k) {
                int nxt = neighbors[p][k];
                dp_next[nxt] += val;
            }
        }
        seq_t tot = 0;
        for (int r = 0; r < ROWS; ++r) tot += dp_next[r];

        cache[idx] = tot;
        memcpy(dp_curr, dp_next, sizeof(dp_curr));
    }
    cache_len = need;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below the defined offset */
    ensure_cache(n);
    return cache[n - 1];
}
