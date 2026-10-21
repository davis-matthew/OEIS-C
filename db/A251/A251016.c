/*
 * A251016  Number of (n+1)X(5+1) 0..2 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements less than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×6 arrays over {0,1,2} satisfying: for
 *            every 2×2 subblock, sum of its diagonal elements is not smaller
 *            than the maximum of its antidiagonal elements
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Number of possible rows: 3^6 = 729. */
#define STATES 729

/* Pre‑computed base‑3 representation of each state (least significant first). */
static uint8_t digits[STATES][6];

/* Adjacency tables: for state i, offsets[i]..offsets[i+1]-1 give the
   indices into edges[] of all permissible following states. */
static int *offsets = NULL;
static int *edges   = NULL;
static int adj_initialized = 0;

/* Build the adjacency tables once. */
static void init_adj(void)
{
    int i, j, k;
    int *cnt = (int *)malloc(STATES * sizeof(int));
    if (!cnt) return;        /* allocation failure → leave uninitialized */

    /* Build the digit tables. */
    for (i = 0; i < STATES; ++i) {
        int v = i;
        for (k = 0; k < 6; ++k) {
            digits[i][k] = v % 3;
            v /= 3;
        }
    }

    /* First pass: count outgoing edges per state. */
    for (i = 0; i < STATES; ++i) cnt[i] = 0;
    for (i = 0; i < STATES; ++i) {
        for (j = 0; j < STATES; ++j) {
            int ok = 1;
            for (k = 0; k < 5; ++k) {
                int a = digits[i][k];
                int b = digits[i][k + 1];
                int c = digits[j][k];
                int d = digits[j][k + 1];
                int ma = (b > c) ? b : c;
                if (a + d < ma) { ok = 0; break; }
            }
            if (ok) ++cnt[i];
        }
    }

    /* Allocate offsets array. */
    offsets = (int *)malloc((STATES + 1) * sizeof(int));
    if (!offsets) { free(cnt); return; }

    offsets[0] = 0;
    for (i = 0; i < STATES; ++i) offsets[i + 1] = offsets[i] + cnt[i];

    int total_edges = offsets[STATES];
    edges = (int *)malloc(total_edges * sizeof(int));
    if (!edges) { free(cnt); free(offsets); return; }

    /* Second pass: store adjacency lists. */
    for (i = 0; i < STATES; ++i) {
        int idx = offsets[i];
        for (j = 0; j < STATES; ++j) {
            int ok = 1;
            for (k = 0; k < 5; ++k) {
                int a = digits[i][k];
                int b = digits[i][k + 1];
                int c = digits[j][k];
                int d = digits[j][k + 1];
                int ma = (b > c) ? b : c;
                if (a + d < ma) { ok = 0; break; }
            }
            if (ok) edges[idx++] = j;
        }
    }

    free(cnt);
    adj_initialized = 1;
}

/* Public function that returns a(n). */
seq_t a(idx_t n)
{
    if (!adj_initialized) init_adj();

    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];
    int i, start, end, next_state;
    idx_t step;
    seq_t sum;

    /* First row: all states are possible. */
    for (i = 0; i < STATES; ++i) dp_cur[i] = 1;

    /* Add n more rows to obtain n+1 rows total. */
    for (step = 0; step < n; ++step) {
        /* Zero next array. */
        for (i = 0; i < STATES; ++i) dp_next[i] = 0;

        /* Propagate counts along allowed transitions. */
        for (i = 0; i < STATES; ++i) {
            seq_t v = dp_cur[i];
            if (!v) continue;
            start = offsets[i];
            end   = offsets[i + 1];
            for (next_state = start; next_state < end; ++next_state) {
                int j = edges[next_state];
                dp_next[j] += v;
            }
        }

        /* Copy next into current for next iteration. */
        for (i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum over all possible last rows. */
    sum = 0;
    for (i = 0; i < STATES; ++i) sum += dp_cur[i];
    return sum;
}
