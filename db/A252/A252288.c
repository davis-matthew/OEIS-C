/*
 * A252288  Number of (n+2)X(3+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 1 or 4.
 * https://oeis.org/A252288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)X5 0..2 arrays whose every 3X3 subblock
 *            has all row, column, diagonal and anti‑diagonal sums distinct
 *            from 0, 1 and 4. Computation is performed by dynamic programming
 *            over the last two rows
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t idx_t;   /* index type */

/* The type seq_t is supplied by the build system; we only use it for
   integer arithmetic. */

#define ROWS 243          /* 3^5 possible 5‑column rows */
#define COLS 5
#define WIN_ST 3          /* number of 3‑column windows per row */

/* Pre‑computed row data */
static uint8_t row_vals[ROWS][COLS];
static uint8_t row_hsum[ROWS][WIN_ST];
static bool   allowed[7];

/* Edge lists (each edge corresponds to a valid triple (a,b,c) of rows) */
static idx_t *edges_src = NULL;
static idx_t *edges_tgt = NULL;
static idx_t  edges_cnt = 0;
static size_t edges_cap = 0;

/* DP arrays */
static seq_t *dp_prev = NULL;
static seq_t *dp_next = NULL;
static idx_t  dp_size = ROWS * ROWS;

/* Memoisation */
static seq_t *memo      = NULL; /* index  n  -> a(n)  (offset 1) */
static idx_t  memo_cap  = 0;
static idx_t  computed_n = 0;   /* highest n for which memo[n] is set */
static idx_t  cur_height = 2;   /* height represented by dp_prev  (initial 2) */

/* ------------------------------------------------------------ */
/* Helper: add an edge to the dynamic edge list                */
static void add_edge(idx_t a, idx_t b, idx_t c)
{
    if (edges_cnt == edges_cap) {
        size_t new_cap = edges_cap ? edges_cap << 1 : 1 << 16;
        edges_src = (idx_t *)realloc(edges_src, new_cap * sizeof(idx_t));
        edges_tgt = (idx_t *)realloc(edges_tgt, new_cap * sizeof(idx_t));
        edges_cap = new_cap;
    }
    edges_src[edges_cnt] = a * ROWS + b;
    edges_tgt[edges_cnt] = b * ROWS + c;
    edges_cnt++;
}

/* ------------------------------------------------------------ */
/* One–time initialisation: build rows, allowed arrays and edges */
static void init_once(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* build all possible 5‑column rows in base‑3 */
    for (idx_t r = 0; r < ROWS; ++r) {
        idx_t tmp = r;
        for (int c = 0; c < COLS; ++c) {
            row_vals[r][c] = tmp % 3;
            tmp /= 3;
        }
        for (int j = 0; j < WIN_ST; ++j)
            row_hsum[r][j] = row_vals[r][j] + row_vals[r][j+1] + row_vals[r][j+2];
    }

    /* allowed sums: all except 0,1,4 */
    for (int s = 0; s < 7; ++s)
        allowed[s] = (s != 0 && s != 1 && s != 4);

    /* enumerate all triples and keep the valid ones */
    for (idx_t a = 0; a < ROWS; ++a)
        for (idx_t b = 0; b < ROWS; ++b)
            for (idx_t c = 0; c < ROWS; ++c) {
                bool ok = true;
                for (int j = 0; j < WIN_ST && ok; ++j) {
                    if (!allowed[row_hsum[a][j]]) { ok = false; continue; }
                    if (!allowed[row_hsum[b][j]]) { ok = false; continue; }
                    if (!allowed[row_hsum[c][j]]) { ok = false; continue; }

                    uint8_t ca = row_vals[a][j]   + row_vals[b][j]   + row_vals[c][j];
                    uint8_t cb = row_vals[a][j+1] + row_vals[b][j+1] + row_vals[c][j+1];
                    uint8_t cc = row_vals[a][j+2] + row_vals[b][j+2] + row_vals[c][j+2];
                    if (!(allowed[ca] && allowed[cb] && allowed[cc]))
                        { ok = false; continue; }

                    uint8_t d1 = row_vals[a][j]   + row_vals[b][j+1] + row_vals[c][j+2];
                    uint8_t d2 = row_vals[a][j+2] + row_vals[b][j+1] + row_vals[c][j];
                    if (!(allowed[d1] && allowed[d2]))
                        { ok = false; continue; }
                }
                if (ok) add_edge(a, b, c);
            }

    /* allocate DP arrays */
    dp_prev = (seq_t *)malloc(dp_size * sizeof(seq_t));
    dp_next = (seq_t *)malloc(dp_size * sizeof(seq_t));

    /* initialise dp_prev for height = 2: all pairs count 1 */
    for (idx_t i = 0; i < dp_size; ++i) dp_prev[i] = (seq_t)1;
    memset(dp_next, 0, dp_size * sizeof(seq_t));

    /* Prepare memo array with room for at least 1024 terms */
    memo_cap = 1024;
    memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    memset(memo, 0, memo_cap * sizeof(seq_t));
    computed_n = 0;
    cur_height = 2;
}

/* ------------------------------------------------------------ */
/* Ensure memo array can hold up to n */
static void ensure_memo_capacity(idx_t n)
{
    if (n < memo_cap) return;
    size_t new_cap = memo_cap;
    while (n >= new_cap) new_cap <<= 1;
    memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    memset(memo + memo_cap, 0, (new_cap - memo_cap) * sizeof(seq_t));
    memo_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset starts at 1 */
    init_once();

    ensure_memo_capacity(n);

    /* Grow DP until we reach the desired n */
    while (computed_n < n) {
        /* perform one DP step: height increases by 1 */
        for (idx_t i = 0; i < dp_size; ++i) dp_next[i] = 0;

        seq_t total = 0;
        for (idx_t e = 0; e < edges_cnt; ++e) {
            idx_t src = edges_src[e];
            idx_t tgt = edges_tgt[e];
            dp_next[tgt] += dp_prev[src];
            total += dp_prev[src];
        }

        /* advance to next height */
        cur_height++;
        computed_n = cur_height - 2;
        memo[computed_n] = total;

        /* swap dp_prev and dp_next for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;   /* dp_next now holds old dp_prev data */
        /* dp_next will be overwritten in the next loop iteration */
    }

    return memo[n];
}
