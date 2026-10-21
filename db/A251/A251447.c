/*
 * A251447  Number of (n+1) X (4+1) 0..2 arrays with no 2 X 2 subblock having
 *          the minimum of its diagonal elements less than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   row_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A251447
 *
 *  a(n) = number of (n+1) × 5 arrays with entries 0..2 such that
 *  for every 2×2 sub‑block the minimum of its diagonal elements
 *  is not smaller than the absolute difference of its
 *  antidiagonal elements.
 *
 *  Algorithm:
 *  - Treat every 5‑tuple over {0,1,2} as a node (243 nodes).
 *  - There is an edge r → s iff for all column positions
 *        min( r[c] , s[c+1] ) >= | r[c+1] - s[c] |   (c = 0..3).
 *  - An array of height n+1 corresponds to a walk of length n in
 *    this graph; a(n) is the total number of such walks.
 *  - Pre‑compute the adjacency lists once; then do a simple
 *    dynamic‑programming walk count of length n.
 *    Complexity is O(n·E) where E ≈ 7 000 edges, easily < 2 s for n ≤ 10 000.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/row_digits.h"

/* number of possible 5‑tuples */
enum { ROWS = 243 };

/* adjacency data: for each row store a list of neighbour indices */
static uint16_t neigh[ROWS][ROWS];   /* at most ROWS neighbours per row */
static uint16_t neigh_cnt[ROWS];
static bool initialized = false;

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Build the adjacency lists once.                                  */
static void init_neighbors(void) {
    if (initialized)
        return;
    initialized = true;

    int rd[5], sd[5];
    for (int r = 0; r < ROWS; ++r) {
        row_digits(r, rd);
        neigh_cnt[r] = 0;
        for (int s = 0; s < ROWS; ++s) {
            row_digits(s, sd);
            bool ok = true;
            for (int c = 0; c < 4 && ok; ++c) {
                int a = rd[c] < sd[c+1] ? rd[c] : sd[c+1];
                int d = rd[c+1] - sd[c];
                if (d < 0) d = -d;
                if (a < d) ok = false;
            }
            if (ok) {
                neigh[r][neigh_cnt[r]++] = (uint16_t)s;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Sequence value: count of walks of length n.                        */
static seq_t dp_arr1[ROWS];
static seq_t dp_arr2[ROWS];

seq_t a(idx_t n)
{
    init_neighbors();          /* adjacency ready */

    /* initialise counts: one way for each starting row */
    for (int i = 0; i < ROWS; ++i)
        dp_arr1[i] = (seq_t)1;

    seq_t *cur  = dp_arr1;
    seq_t *next = dp_arr2;

    for (idx_t step = 0; step < n; ++step) {
        /* zero the next layer */
        for (int i = 0; i < ROWS; ++i)
            next[i] = 0;

        /* propagate counts through all edges */
        for (int r = 0; r < ROWS; ++r) {
            seq_t val = cur[r];
            if (!val) continue;
            uint16_t deg = neigh_cnt[r];
            for (uint16_t j = 0; j < deg; ++j) {
                int s = neigh[r][j];
                next[s] += val;
            }
        }

        /* swap current and next for the following step */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum over all possible ending rows */
    seq_t total = 0;
    for (int i = 0; i < ROWS; ++i)
        total += cur[i];

    return total;
}
