/*
 * A251448  Number of (n+1)X(5+1) 0..2 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements less than the absolute difference
 *          of its antidiagonal elements.
 * https://oeis.org/A251448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) × 6 arrays over {0,1,2} with no 2×2
 *            subblock having the minimum of its diagonal elements less than
 *            the absolute difference of its antidiagonal elements. Computed
 *            by dynamic programming over rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define STATES 729 /* 3^6, number of distinct 6‑cell rows */

/* Pre‑computed base‑3 digits for every row state */
static uint8_t state_digits[STATES][6];
/* Concatenated adjacency lists */
static uint16_t *adj_edges = NULL;
/* offsets[v] gives the start index of adjacency list for row v */
static size_t *offsets = NULL;
/* Flag to indicate whether the adjacency lists have been built */
static bool adjacency_built = false;

/* Check whether two rows are compatible, i.e. satisfied the
   2×2 restriction for every pair of adjacent columns. */
static inline bool check_compat(int v, int w)
{
    const uint8_t *dv = state_digits[v];
    const uint8_t *dw = state_digits[w];
    for (int j = 0; j < 5; ++j) {
        uint8_t a00 = dv[j];
        uint8_t a01 = dv[j + 1];
        uint8_t a10 = dw[j];
        uint8_t a11 = dw[j + 1];
        uint8_t min_diag = a00 < a11 ? a00 : a11;
        uint8_t diff = a01 > a10 ? a01 - a10 : a10 - a01;
        if (min_diag < diff) return false;
    }
    return true;
}

/* Build the adjacency lists once.  This is shared by all calls to a(). */
static void build_adjacent(void)
{
    /* Pre‑compute the 6 base‑3 digits for every possible row (state). */
    for (int id = 0; id < STATES; ++id) {
        int x = id;
        for (int pos = 5; pos >= 0; --pos) {
            state_digits[id][pos] = (uint8_t)(x % 3);
            x /= 3;
        }
    }

    /* First pass: compute out‑degrees and total number of edges. */
    int deg[STATES] = {0};
    for (int v = 0; v < STATES; ++v) {
        for (int w = 0; w < STATES; ++w) {
            if (check_compat(v, w))
                ++deg[v];
        }
    }

    /* Compute offsets array. */
    offsets = (size_t *)malloc((STATES + 1) * sizeof(size_t));
    if (!offsets) exit(1);
    offsets[0] = 0;
    size_t total_edges = 0;
    for (int v = 0; v < STATES; ++v) {
        total_edges += deg[v];
        offsets[v + 1] = total_edges;
    }

    /* Allocate concatenated adjacency list and fill it. */
    adj_edges = (uint16_t *)malloc(total_edges * sizeof(uint16_t));
    if (!adj_edges) exit(1);
    size_t pos[STATES];
    for (int v = 0; v < STATES; ++v) pos[v] = offsets[v];

    for (int v = 0; v < STATES; ++v) {
        for (int w = 0; w < STATES; ++w) {
            if (check_compat(v, w))
                adj_edges[pos[v]++] = (uint16_t)w;
        }
    }

    adjacency_built = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* sequence is offset 1; below offset is irrelevant */

    if (!adjacency_built)
        build_adjacent();

    /* DP arrays: counts of arrays ending in each state. */
    seq_t cur[STATES];
    seq_t nxt[STATES];
    for (int i = 0; i < STATES; ++i)
        cur[i] = (seq_t)1; /* single row of length 1 */

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i)
            nxt[i] = (seq_t)0;

        for (int v = 0; v < STATES; ++v) {
            seq_t cnt = cur[v];
            if (cnt == (seq_t)0) continue;
            size_t start = offsets[v];
            size_t end   = offsets[v + 1];
            for (size_t p = start; p < end; ++p)
                nxt[adj_edges[p]] += cnt;
        }
        /* Copy nxt into cur for next iteration */
        for (int i = 0; i < STATES; ++i)
            cur[i] = nxt[i];
    }

    /* Sum over all ending states to obtain the total number. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATES; ++i)
        total += cur[i];

    return total;
}
