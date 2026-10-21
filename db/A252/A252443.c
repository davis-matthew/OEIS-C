/*
 * A252443  Number of (n+2)X(3+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum less than 3.
 * https://oeis.org/A252443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm counts a(n): the number of (n+2)×5 arrays over {0,1,2}
   that satisfy the constraint that for every 3×3 subblock the sums of
   its rows, columns, main diagonal and anti‑diagonal are < 3.
   The computation uses dynamic programming over pairs of consecutive
   rows, restricted to those rows whose every 3‑digit window sums to ≤ 2
   (there are only a few hundred such rows).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 3^k for k = 0..4 */
static const int powers3[5] = { 1, 3, 9, 27, 81 };

/* All 243 possible rows (5‑digit base‑3 vectors) */
static unsigned char row_vals[243][5];

/* List of row indices that satisfy the 3‑digit sum constraint.
   These are the only rows that may appear in a valid array. */
static unsigned char *valid_row_ids = NULL;   /* length R   */
static idx_t R = 0;                           /* number of valid rows */

/* Transition tables:
   trans_start[p] -- start index in trans_pool for pair index p
   trans_len[p]   -- number of transitions for pair p
   trans_pool     -- concatenated lists of destination row ids
   All indices refer to the 0..R‑1 range of valid rows. */
static size_t *trans_start = NULL;
static unsigned char *trans_pool = NULL;
static size_t *trans_len = NULL;

/* Precompute all 243 rows once */
static void init_row_vals(void)
{
    for (int r = 0; r < 243; ++r) {
        int t = r;
        for (int c = 0; c < 5; ++c) {
            row_vals[r][4 - c] = t % 3;   /* most significant digit first */
            t /= 3;
        }
    }
}

/* Check if a row satisfies that every 3‑digit window sums to ≤ 2 */
static bool row_ok(int r)
{
    const unsigned char *row = row_vals[r];
    for (int cs = 0; cs <= 2; ++cs) {
        int sum = row[cs] + row[cs + 1] + row[cs + 2];
        if (sum > 2) return false;
    }
    return true;
}

/* Build the array of valid row ids */
static void init_valid_rows(void)
{
    /* Count first */
    idx_t cnt = 0;
    for (int r = 0; r < 243; ++r)
        if (row_ok(r)) ++cnt;
    R = cnt;

    valid_row_ids = (unsigned char *)malloc(R * sizeof(unsigned char));
    if (!valid_row_ids) exit(1);
    idx_t pos = 0;
    for (int r = 0; r < 243; ++r)
        if (row_ok(r)) valid_row_ids[pos++] = r;
}

/* Test if a 3×3 block defined by rows a,b,c is valid */
static bool valid_triple(int a, int b, int c)
{
    const unsigned char *ra = row_vals[a];
    const unsigned char *rb = row_vals[b];
    const unsigned char *rc = row_vals[c];

    for (int cs = 0; cs <= 2; ++cs) {
        /* Row sums of the 3x3 block */
        if (ra[cs] + ra[cs + 1] + ra[cs + 2] > 2) return false;
        if (rb[cs] + rb[cs + 1] + rb[cs + 2] > 2) return false;
        if (rc[cs] + rc[cs + 1] + rc[cs + 2] > 2) return false;

        /* Column sums */
        if (ra[cs]   + rb[cs]   + rc[cs]   > 2) return false;
        if (ra[cs+1] + rb[cs+1] + rc[cs+1] > 2) return false;
        if (ra[cs+2] + rb[cs+2] + rc[cs+2] > 2) return false;

        /* Main diagonal */
        if (ra[cs] + rb[cs+1] + rc[cs+2] > 2) return false;

        /* Anti‑diagonal */
        if (ra[cs+2] + rb[cs+1] + rc[cs] > 2) return false;
    }
    return true;
}

/* Build transition tables once */
static void init_transitions(void)
{
    /* trans_len for each pair */
    trans_len = (size_t *)calloc(R * R, sizeof(size_t));
    if (!trans_len) exit(1);

    /* Count transitions */
    for (idx_t i = 0; i < R; ++i) {
        for (idx_t j = 0; j < R; ++j) {
            size_t cnt = 0;
            int a = valid_row_ids[i];
            int b = valid_row_ids[j];
            for (idx_t k = 0; k < R; ++k) {
                int c = valid_row_ids[k];
                if (valid_triple(a, b, c)) ++cnt;
            }
            trans_len[i * R + j] = cnt;
        }
    }

    /* Prefix sums to get positions */
    size_t total = 0;
    trans_start = (size_t *)malloc((R * R + 1) * sizeof(size_t));
    if (!trans_start) exit(1);
    trans_start[0] = 0;
    for (idx_t p = 0; p < R * R; ++p) {
        total += trans_len[p];
        trans_start[p + 1] = total;
    }

    trans_pool = (unsigned char *)malloc(total * sizeof(unsigned char));
    if (!trans_pool) exit(1);

    /* Fill transition pool */
    for (idx_t i = 0; i < R; ++i) {
        for (idx_t j = 0; j < R; ++j) {
            size_t pos = trans_start[i * R + j];
            int a = valid_row_ids[i];
            int b = valid_row_ids[j];
            for (idx_t k = 0; k < R; ++k) {
                int c = valid_row_ids[k];
                if (valid_triple(a, b, c))
                    trans_pool[pos++] = (unsigned char)k;
            }
        }
    }
}

/* Global initialization flag */
static bool tables_initialised = false;

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise everything on first call */
    if (!tables_initialised) {
        init_row_vals();
        init_valid_rows();
        init_transitions();
        tables_initialised = true;
    }

    /* DP arrays over pairs of last two rows. */
    size_t sz = R * R;
    seq_t *dp   = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp || !next) { free(dp); free(next); exit(1); }

    /* Base: any pair of valid rows is allowed before we have a third row */
    for (size_t p = 0; p < sz; ++p) dp[p] = (seq_t)1;

    /* Add the remaining rows (there are n extra rows beyond the first two) */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero next */
        for (size_t p = 0; p < sz; ++p) next[p] = (seq_t)0;

        for (size_t p = 0; p < sz; ++p) {
            seq_t val = dp[p];
            if (val == (seq_t)0) continue;

            size_t start = trans_start[p];
            size_t end   = trans_start[p + 1];
            for (size_t t = start; t < end; ++t) {
                unsigned char k = trans_pool[t];
                size_t new_p = ((size_t)(p % R)) * R + k;  /* last two rows become (prev_row, k) */
                next[new_p] += val;
            }
        }
        /* swap dp and next */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* Sum all entries to get the total number of arrays */
    seq_t ans = (seq_t)0;
    for (size_t p = 0; p < sz; ++p) ans += dp[p];

    free(dp);
    free(next);
    return ans;
}
