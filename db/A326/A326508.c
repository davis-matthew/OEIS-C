/*
 * A326508  Number of (binary) max-heaps on n elements from the set {0,1}
 *          containing exactly seven 0's.
 * https://oeis.org/A326508
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of binary max‑heaps on n nodes from {0,1}
 *            with exactly seven 0's. Let f[n][k] be the number of heaps of
 *            size n with k zeros. Recurrence: f[0][0] = 1, f[0][k>0] = 0. for
 *            n > 0: if k == n // all nodes zero f[n][k] = 1 else let L = size
 *            of left subtree of a complete tree of n node...
 * Range:     u8 n <= 17, u16 n <= 36, u32 n <= 166, u64 n <= 3826,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----- standard includes ----- */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ----- helper: floor(log2) for idx_t ----- */
static inline idx_t floor_log2_uint64(idx_t x) {
    /* x > 0 */
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

/* ----- helper: size of left subtree of a complete binary tree with n nodes ----- */
static inline idx_t left_subtree_size(idx_t n) {
    if (n <= 1) return 0;
    idx_t h = floor_log2_uint64(n);           /* height of the tree */
    idx_t nodes_above_last = ((idx_t)1 << h) - 1;     /* all nodes except last level */
    idx_t nodes_last = n - nodes_above_last;           /* nodes on the last level */
    idx_t left_last = ((idx_t)1 << (h-1));             /* max nodes that can fit on left side of last level */
    if (nodes_last < left_last) left_last = nodes_last;
    /* nodes above last level that belong to left subtree */
    idx_t left_above = ((idx_t)1 << (h-1)) - 1;
    return left_above + left_last;
}

/* ----- DP table ----- */
static seq_t *dp = NULL;        /* dp[s][k] stored as dp[s*(KMAX+1)+k] */
static idx_t curr_max = 0;       /* largest n for which dp has been built */
#define KMAX 7

/* ----- ensure dp table covers up to n ----- */
static void ensure_size(idx_t n) {
    if (curr_max >= n) return;

    size_t old_cells = (size_t)(curr_max + 1) * (KMAX + 1);
    size_t new_cells = (size_t)(n + 1) * (KMAX + 1);

    if (dp == NULL) {
        dp = (seq_t *)calloc(new_cells, sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(dp, new_cells * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort silently (unlikely in test harness) */
            exit(EXIT_FAILURE);
        }
        dp = tmp;
        /* zero new portion */
        memset(dp + old_cells, 0, (new_cells - old_cells) * sizeof(seq_t));
    }

    /* dp[0][0] already zero from calloc; set dp[0][0] = 1 */
    dp[0] = (seq_t)1;

    /* build DP for sizes curr_max+1 .. n */
    for (idx_t sz = curr_max + 1; sz <= n; ++sz) {
        idx_t L = left_subtree_size(sz);
        idx_t R = sz - 1 - L;
        for (idx_t k = 0; k <= KMAX; ++k) {
            seq_t *dp_sz = dp + sz * (KMAX + 1);
            if (k == sz) {
                dp_sz[k] = (seq_t)1;      /* all zeros in the whole tree */
            } else {
                seq_t sum = (seq_t)0;
                for (idx_t i = 0; i <= k; ++i) {
                    seq_t *dp_L = dp + L * (KMAX + 1);
                    seq_t *dp_R = dp + R * (KMAX + 1);
                    sum += dp_L[i] * dp_R[k - i];
                }
                dp_sz[k] = sum;
            }
        }
    }
    curr_max = n;
}

/* ----- required function ----- */
seq_t a(idx_t n) {
    if (n < 7) return (seq_t)0;   /* offset: a(7) is the first term */
    ensure_size(n);
    return dp[n * (KMAX + 1) + KMAX];
}
