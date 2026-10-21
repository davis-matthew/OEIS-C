/*
 * A250449  Number of (n+1)X(3+1) 0..2 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays with entries 0..2 that satisfy
 *            the min­–based monotonicity constraints: (1)
 *            min(x(i,j),x(i,j-1)) is nondecreasing for increasing i, (2)
 *            min(x(i,j),x(i-1,j)) is nondecreasing for increasing j
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation pre‑computes all allowed row transitions
   (there are 3⁴ = 81 possible rows) and then uses a small
   dynamic‑programming memoisation.  The algorithm is O(n · 81²)
   with negligible constant factors. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 81 possible rows (4 digits in base‑3) */
static int row_vals[81][4];

/* Transition data: for each previous row p, out_adj[p] lists all
   rows q that can follow p, and out_deg[p] stores the number of
   such q. */
static unsigned char out_deg[81];
static int out_adj[81][81];   /* maximum possible out_degree is 81 */

/* Initialise row_vals and transition tables. */
static void init_tables(void)
{
    /* decode each 4‑digit base‑3 row into its 4 components */
    for (int idx = 0; idx < 81; ++idx) {
        int tmp = idx;
        row_vals[idx][3] = tmp % 3; tmp /= 3;
        row_vals[idx][2] = tmp % 3; tmp /= 3;
        row_vals[idx][1] = tmp % 3; tmp /= 3;
        row_vals[idx][0] = tmp % 3;
    }

    /* build adjacency lists */
    for (int p = 0; p < 81; ++p) {
        int *pv = row_vals[p];
        out_deg[p] = 0;
        for (int q = 0; q < 81; ++q) {
            int *qv = row_vals[q];
            bool ok = true;

            /* i‑direction constraint:  min(qj, qj‑1) ≥ min(pj, pj‑1) */
            for (int j = 1; j < 4 && ok; ++j) {
                int fp = pv[j] < pv[j-1] ? pv[j] : pv[j-1];
                int fq = qv[j] < qv[j-1] ? qv[j] : qv[j-1];
                if (fq < fp) ok = false;
            }
            if (!ok) continue;

            /* j‑direction constraint:  min(qj, pj) ≥ min(qj‑1, pj‑1) */
            for (int j = 1; j < 4 && ok; ++j) {
                int gprev  = qv[j] < pv[j]  ? qv[j]  : pv[j];
                int gprevp = qv[j-1] < pv[j-1] ? qv[j-1] : pv[j-1];
                if (gprev < gprevp) ok = false;
            }
            if (ok) {
                out_adj[p][out_deg[p]++] = q;
            }
        }
    }
}

/* DP buffers and state */
static seq_t dp[2][81];
static int dp_cur = 0, dp_nxt = 1;
static bool dp_initialized = false;

/* Memoisation table for a(n) */
static seq_t *memo = NULL;
static size_t memo_cap = 0;
static size_t memo_used = 0;   /* highest n stored (memo_used >= 0) */

/* Grow memoisation array to at least required capacity. */
static void memo_grow_needed(size_t required)
{
    if (required <= memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap << 1 : 64;
    while (new_cap < required) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *new_mem = new seq_t[new_cap];
    if (memo) {
        for (size_t i = 0; i <= memo_used; ++i)
            new_mem[i] = memo[i];          /* copy existing values */
        delete[] memo;
    }
#else
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
#endif
    memo = new_mem;
    memo_cap = new_cap;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, harmless */

    /* initialise tables once */
    static bool tables_initialised = false;
    if (!tables_initialised) {
        init_tables();
        tables_initialised = true;
    }

    /* initialise memoisation storage if first call */
    if (memo_cap == 0) {
        memo_cap = 64;
#ifdef __cplusplus
        memo = new seq_t[memo_cap];
#else
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
#endif
        memo_used = 0;
    }

    /* ensure storage large enough for requested n */
    memo_grow_needed(n + 1);   /* index n will be used */

    /* already known? */
    if ((size_t)n <= memo_used) return memo[n];

    /* initialise DP buffers on first invocation */
    if (!dp_initialized) {
        for (int i = 0; i < 81; ++i) dp[0][i] = (seq_t)1;  /* height 1 */
        for (int i = 0; i < 81; ++i) dp[1][i] = (seq_t)0;
        dp_initialized = true;
    }

    /* compute successive steps until n */
    for (idx_t step = memo_used + 1; step <= n; ++step) {
        /* zero next buffer */
        for (int i = 0; i < 81; ++i) dp[dp_nxt][i] = (seq_t)0;

        /* transitions from current rows to next rows */
        for (int p = 0; p < 81; ++p) {
            seq_t val = dp[dp_cur][p];
            if (val == (seq_t)0) continue;
            unsigned char dg = out_deg[p];
            for (int t = 0; t < dg; ++t) {
                int q = out_adj[p][t];
                dp[dp_nxt][q] = dp[dp_nxt][q] + val;
            }
        }

        /* sum all ways for arrays of height (step+1) */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 81; ++i) total = total + dp[dp_nxt][i];

        /* store memoised result */
        memo[step] = total;
        memo_used = (size_t)step;

        /* swap buffers for next iteration */
        int tmp = dp_cur; dp_cur = dp_nxt; dp_nxt = tmp;
    }

    return memo[n];
}
