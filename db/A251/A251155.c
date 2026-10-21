/*
 * A251155  Number of (n+1)X(5+1) 0..2 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A251155: Number of (n+1)×6 arrays with entries in {0,1,2} such that
 * no 2×2 subblock has the minimum of its diagonal elements greater than
 * the absolute difference of its anti-diagonal elements.
 *
 * The implementation performs a dynamic programming over the rows of the
 * array.  There are 3^6 = 729 possible rows.  Two rows a and b are
 * compatible iff for every adjacent column j (0 ≤ j < 5) we have
 *
 *      min(a[j], b[j+1]) ≤ |a[j+1] − b[j]|
 *
 * The number of (n+1)-row arrays is the sum, over all ending rows, of the
 * number of ways to reach them after n steps.  A memoised table of the
 * sequence values is kept to honour the requirement that calls may arrive
 * in any order.
 *
 * The code uses the helper functions seq_alloc() and seq_realloc() provided
 * by the build system to correctly construct arrays of potentially
 * arbitrary-precision integer type `seq_t`.  This avoids undefined behaviour
 * that would arise from using malloc() / realloc() on a class type in C++.
 */

#include <stddef.h>
#include <stdint.h>

/* Helpers for dynamic allocation of seq_t objects (provided by the build). */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Constants */
#define ROWS 729          /* 3^6 possible rows */
#define COLS 6

/* ---------------------------------------------------------------------- */
static void build_rows(unsigned char rows[ROWS][COLS]) {
    for (int code = 0; code < ROWS; ++code) {
        int x = code;
        for (int d = COLS - 1; d >= 0; --d) {
            rows[code][d] = (unsigned char)(x % 3);
            x /= 3;
        }
    }
}

/* ---------------------------------------------------------------------- */
static size_t *adj_len = NULL;   /* number of neighbours for each row */
static int   **adj_list = NULL;  /* adjacency lists */

/* Pre‑compute all compatible pairs of rows. */
static void build_adj(const unsigned char rows[ROWS][COLS]) {
    adj_len = (size_t *)malloc(ROWS * sizeof(size_t));
    adj_list = (int **)malloc(ROWS * sizeof(int *));
    for (int i = 0; i < ROWS; ++i) {
        size_t cap = 0, len = 0;
        int *list = NULL;
        for (int j = 0; j < ROWS; ++j) {
            int ok = 1;
            for (int p = 0; p < COLS - 1 && ok; ++p) {
                int a = rows[i][p];
                int b = rows[i][p + 1];
                int c = rows[j][p];
                int d = rows[j][p + 1];
                int min_diag = (a < d) ? a : d;
                int diff = (b > c) ? (b - c) : (c - b);
                if (min_diag > diff) ok = 0;
            }
            if (ok) {
                if (len == cap) {
                    cap = cap ? cap * 2 : 8;
                    list = (int *)realloc(list, cap * sizeof(int));
                }
                list[len++] = j;
            }
        }
        adj_list[i] = list;
        adj_len[i] = len;
    }
}

/* ---------------------------------------------------------------------- */
/* Main sequence function.  It is idempotent and memoised. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset 1 – a(1) is first term */

    /* Static data structures, initialised on first call. */
    static int initialized = 0;
    static unsigned char rows[ROWS][COLS];
    static seq_t *cur = NULL, *next = NULL;
    static seq_t *seq_vals = NULL;       /* array of computed a(k) */
    static size_t seq_vals_cap = 0;      /* current capacity of seq_vals */
    static idx_t seq_len = 0;            /* largest k for which seq_vals[k] is set */

    if (!initialized) {
        /* One‑time setup */
        build_rows(rows);
        build_adj(rows);

        cur  = seq_alloc(ROWS);   /* constructs all elements to zero */
        next = seq_alloc(ROWS);

        for (int i = 0; i < ROWS; ++i) cur[i] = 1;   /* one copy of each first row */
        seq_vals = NULL;
        seq_vals_cap = 0;
        seq_len = 0;
        initialized = 1;
    }

    /* Extend the cached sequence up to n if necessary. */
    if (n > seq_len) {
        /* Ensure seq_vals has capacity for indices 0 .. n. */
        if (seq_vals_cap <= n) {
            seq_vals = seq_realloc(seq_vals, seq_vals_cap, n + 1);
            seq_vals_cap = n + 1;
        }

        for (idx_t step = seq_len + 1; step <= n; ++step) {
            /* Zero next. */
            for (int i = 0; i < ROWS; ++i) next[i] = 0;

            /* Transition: for each compatibility pair (i, j) add cur[i] to next[j]. */
            for (int i = 0; i < ROWS; ++i) {
                seq_t val = cur[i];
                if (val == 0) continue;
                size_t len = adj_len[i];
                int *list = adj_list[i];
                for (size_t k = 0; k < len; ++k) {
                    int j = list[k];
                    next[j] += val;
                }
            }

            /* Sum all counts for arrays of height step + 1. */
            seq_t total = 0;
            for (int i = 0; i < ROWS; ++i) total += next[i];
            seq_vals[step] = total;

            /* Prepare for next iteration. */
            seq_t *tmp = cur; cur = next; next = tmp;
        }
        seq_len = n;
    }

    return seq_vals[n];
}
