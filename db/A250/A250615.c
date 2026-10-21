/*
 * A250615  Number of (n+1)X(3+1) 0..2 arrays with nondecreasing maximum of
 *          every two consecutive values in every row and column.
 * https://oeis.org/A250615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays with elements 0..2 such that the
 *            maximum of each pair of consecutive entries in every row and
 *            every column is nondecreasing (OEIS A250615)
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define NROWS 52
#define STATE (NROWS * NROWS)

/* Count of admissible rows and all per‑state data.  All arrays are static
   and allocated only once. */
static int      row_vals[NROWS][4];   /* admissible rows, indices 0..3 */
static int      outcnt[STATE];        /* number of allowed successors per state */
static int     *adj_start;            /* offset into adj_next where successors
                                       for state i begin; size STATE+1   */
static int     *adj_next;             /* flattened list of successor states */
static seq_t   *dp;                   /* DP values for current step     */
static seq_t   *nextdp;               /* DP values for next step        */
static bool     init_done = false;

/* -------------------------------------------------------------
   Build list of admissible rows (52 rows total).  A row consists
   of four entries a0..a3 in {0,1,2} such that
          max(a0,a1) <= max(a1,a2) <= max(a2,a3)
   ------------------------------------------------------------- */
static void init_rows(void)
{
    int idx = 0;
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d)
                {
                    int m0 = a > b ? a : b;
                    int m1 = b > c ? b : c;
                    int m2 = c > d ? c : d;
                    if (m0 <= m1 && m1 <= m2)
                    {
                        row_vals[idx][0] = a;
                        row_vals[idx][1] = b;
                        row_vals[idx][2] = c;
                        row_vals[idx][3] = d;
                        ++idx;
                    }
                }
    /* idx should be 52 */
}

/* -------------------------------------------------------------
   Build transition graph between states.
   A state is a pair (p,q) of consecutive rows.  From state (p,q)
   we may go to state (q,r) iff, for every column j,
          max(row_p[j],row_q[j]) <= max(row_q[j],row_r[j])
   ------------------------------------------------------------- */
static void init_transitions(void)
{
    long long total_edges = 0;
    int p, q, r, j;

    /* First pass: count outgoing edges per state, sum total edges. */
    for (p = 0; p < NROWS; ++p)
        for (q = 0; q < NROWS; ++q)
        {
            int state = p * NROWS + q;
            int cnt = 0;
            for (r = 0; r < NROWS; ++r)
            {
                bool ok = true;
                for (j = 0; j < 4; ++j)
                {
                    int c1 = row_vals[p][j] > row_vals[q][j] ?
                             row_vals[p][j] : row_vals[q][j];
                    int c2 = row_vals[q][j] > row_vals[r][j] ?
                             row_vals[q][j] : row_vals[r][j];
                    if (c1 > c2) { ok = false; break; }
                }
                if (ok) ++cnt;
            }
            outcnt[state] = cnt;
            total_edges += cnt;
        }

    /* Allocate adjacency tables. */
    adj_start = (int *)malloc((STATE + 1) * sizeof(int));
    adj_next  = (int *)malloc((size_t)total_edges * sizeof(int));

    /* Set starting indices for each state's successor list. */
    int pos = 0;
    for (int i = 0; i < STATE; ++i)
    {
        adj_start[i] = pos;
        pos += outcnt[i];
    }
    adj_start[STATE] = pos;  /* sentinel */

    /* Second pass: fill adjacency lists. */
    pos = 0;
    for (p = 0; p < NROWS; ++p)
        for (q = 0; q < NROWS; ++q)
        {
            int state = p * NROWS + q;
            for (r = 0; r < NROWS; ++r)
            {
                bool ok = true;
                for (j = 0; j < 4; ++j)
                {
                    int c1 = row_vals[p][j] > row_vals[q][j] ?
                             row_vals[p][j] : row_vals[q][j];
                    int c2 = row_vals[q][j] > row_vals[r][j] ?
                             row_vals[q][j] : row_vals[r][j];
                    if (c1 > c2) { ok = false; break; }
                }
                if (ok)
                {
                    adj_next[pos++] = q * NROWS + r;
                }
            }
        }
}

/* -------------------------------------------------------------
   Initialise all static data once.  Subsequent calls are fast.
   ------------------------------------------------------------- */
static void init_globals(void)
{
    if (init_done) return;

    init_rows();
    init_transitions();

    dp      = (seq_t *)malloc(STATE * sizeof(seq_t));
    nextdp  = (seq_t *)malloc(STATE * sizeof(seq_t));

    init_done = true;
}

/* -------------------------------------------------------------
   Main function computing a(n).  The sequence starts at n=1.
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_globals();

    /* n = 1 corresponds to 2×4 arrays.  There are 52 admissible rows,
       so the total number of arrays is 52² = 2704. */
    if (n == 1) return (seq_t)2704;

    /* Number of rows in the array is n+1. */
    idx_t rows = n + 1;

    int i;

    /* Start with all 52² possible pairs for the first two rows. */
    for (i = 0; i < STATE; ++i) dp[i] = (seq_t)1;

    /* Process each additional row (from the 3rd up to the last). */
    for (idx_t step = 3; step <= rows; ++step)
    {
        /* Reset nextdp to zero. */
        for (i = 0; i < STATE; ++i) nextdp[i] = 0;

        /* Transition through the graph. */
        for (i = 0; i < STATE; ++i)
        {
            seq_t val = dp[i];
            if (val == 0) continue;
            int start = adj_start[i];
            int end   = adj_start[i + 1];
            for (int k = start; k < end; ++k)
            {
                int t = adj_next[k];
                nextdp[t] += val;
            }
        }

        /* Swap dp and nextdp for the next iteration. */
        seq_t *tmp = dp;
        dp = nextdp;
        nextdp = tmp;
    }

    /* Sum all counts of final states. */
    seq_t result = 0;
    for (i = 0; i < STATE; ++i) result += dp[i];
    return result;
}
