/*
 * A251164  Number of (n+1)X(5+1) 0..3 arrays with no 2X2 subblock having the
 *          maximum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This sequence is defined by a state‑transition DP over * 6‑digit
 *            base‑4 rows. a(n) counts the number of (n+1)×6 arrays of *
 *            digits 0..3 with no 2×2 subblock having the maximum of its
 *            diagonal * elements greater than the absolute difference of its
 *            antidiagonal * elements. The DP keeps track of the...
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>   /* for memset */

/* ------------------------------------------------------------------ */
/* Constants                                                     */
#define STATES 4096U   /* 4^6 possible rows */

/* ------------------------------------------------------------------ */
/* Global data for the transition graph and DP                     */
static uint8_t        row_digits[STATES][6];   /* base‑4 digits of each row   */
static uint16_t      *edge_next   = 0;        /* flat list of successors    */
static uint32_t      *edge_offset = 0;        /* start index for each row   */
static uint16_t      *edge_deg    = 0;        /* degree of each row         */

static seq_t        *dp_cur  = 0;            /* current DP layer           */
static seq_t        *dp_next = 0;            /* next DP layer              */

static seq_t        *memo_arr    = 0;        /* memoised a(n) values       */
static idx_t         memo_size   = 0;        /* size of memo_arr           */
static idx_t         last_step   = 0;        /* maximum n computed so far  */
static bool          graph_inited = false;  /* guard for graph init        */

/* ------------------------------------------------------------------ */
/* Helper: test if two rows (by index) can be adjacent.             */
static bool rows_ok(uint16_t a, uint16_t b)
{
    const uint8_t *ra = row_digits[a];
    const uint8_t *rb = row_digits[b];
    for (int pos = 0; pos < 5; ++pos) {
        uint8_t x  = ra[pos];
        uint8_t y  = ra[pos+1];
        uint8_t z  = rb[pos];
        uint8_t w  = rb[pos+1];
        uint8_t diff   = (y > z) ? (y - z) : (z - y);
        uint8_t maxdiag = (x > w) ? x : w;
        if (maxdiag > diff)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Build the transition graph once.                                 */
static void init_graph(void)
{
    /* Pre‑compute the 6 digits of every row (base‑4). */
    for (uint16_t r = 0; r < STATES; ++r) {
        uint16_t t = r;
        for (int pos = 0; pos < 6; ++pos) {
            row_digits[r][pos] = (uint8_t)(t & 3U);
            t >>= 2U;
        }
    }

    /* Allocate degrees and offsets. */
    edge_deg    = (uint16_t *)malloc(STATES * sizeof(uint16_t));
    edge_offset = (uint32_t *)malloc((STATES + 1) * sizeof(uint32_t));

    /* Count edges and the total number of edges. */
    uint32_t total_edges = 0;
    for (uint16_t i = 0; i < STATES; ++i) {
        uint16_t cnt = 0;
        for (uint16_t j = 0; j < STATES; ++j)
            if (rows_ok(i, j))
                ++cnt;
        edge_deg[i]    = cnt;
        total_edges   += cnt;
    }

    /* Build offsets. */
    edge_offset[0] = 0;
    for (uint16_t i = 0; i < STATES; ++i)
        edge_offset[i+1] = edge_offset[i] + edge_deg[i];

    /* Allocate flat edge list. */
    edge_next = (uint16_t *)malloc(total_edges * sizeof(uint16_t));

    /* Temporary cursor to fill the edge list. */
    uint32_t *cursor = (uint32_t *)malloc(STATES * sizeof(uint32_t));
    for (uint16_t i = 0; i < STATES; ++i)
        cursor[i] = edge_offset[i];

    /* Second pass: fill edges. */
    for (uint16_t i = 0; i < STATES; ++i) {
        for (uint16_t j = 0; j < STATES; ++j) {
            if (rows_ok(i, j))
                edge_next[cursor[i]++] = j;
        }
    }
    free(cursor);

    /* Allocate DP arrays and initialise them to 0. */
    dp_cur  = (seq_t *)calloc(STATES, sizeof(seq_t));
    dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));

    /* Each row can start a new array: dp_cur[i] = 1. */
    for (uint16_t i = 0; i < STATES; ++i)
        dp_cur[i] = (seq_t)1U;

    graph_inited = true;
}

/* ------------------------------------------------------------------ */
/* Ensure the memoisation array is large enough.                     */
static void ensure_memo_size(idx_t need)
{
    if (memo_size > need)
        return;
    idx_t new_size = need + 1;
    seq_t *new_arr = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (memo_arr != 0) {
        /* Copy existing values. */
        for (idx_t i = 0; i < memo_size; ++i)
            new_arr[i] = memo_arr[i];
        free(memo_arr);
    }
    memo_arr  = new_arr;
    memo_size = new_size;
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                         */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 we return 0. */
    if (n == 0) return (seq_t)0U;

    if (!graph_inited)
        init_graph();

    if (n > last_step) {
        /* Grow memo table if necessary. */
        ensure_memo_size(n);

        /* Incrementally compute from last_step+1 up to n. */
        for (idx_t step = last_step + 1; step <= n; ++step) {
            /* Transition from dp_cur (height step) to dp_next (height step+1). */
            for (uint16_t i = 0; i < STATES; ++i) {
                seq_t ways = dp_cur[i];
                if (ways == (seq_t)0U) continue;
                uint32_t start = edge_offset[i];
                uint32_t end   = edge_offset[i+1];
                for (uint32_t p = start; p < end; ++p)
                    dp_next[edge_next[p]] += ways;
            }

            /* Sum total number of arrays of height step+1 (i.e. a(step)). */
            seq_t total = (seq_t)0U;
            for (uint16_t i = 0; i < STATES; ++i)
                total += dp_next[i];
            memo_arr[step] = total;

            /* Prepare for next iteration. */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
            /* Zero the new dp_next array. */
            for (uint16_t i = 0; i < STATES; ++i)
                dp_next[i] = (seq_t)0U;
        }

        last_step = n;
    }

    return memo_arr[n];
}
