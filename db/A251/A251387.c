/*
 * A251387  Number of (n+1) X (5+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 2 3 4 5 or 6.
 * https://oeis.org/A251387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A251387 by dynamic programming over row states
   (n+1) rows of width 6.  Each transition between two rows is allowed
   iff all 2×2 blocks formed by the two rows have a sum in {2,3,4,5,6}.
   The DP vector is updated step‑by‑step; a(n) is the sum of the
   entries after n transitions (i.e. for n+1 rows). */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* number of possible rows of length 6 with digits 0..2 */
#define N_STATES 729
#define WIDTH    6          /* number of columns */
#define BLOCKS   (WIDTH-1)  /* number of 2×2 blocks per pair of rows */

/* --------------------------------------------------------------------- */
/* Static data structures                                               */
/* --------------------------------------------------------------------- */
static uint8_t row_vals[N_STATES][WIDTH];   /* decoded base‑3 representation */

static int *adjacency = NULL;   /* contiguous adjacency list */
static int *adj_start  = NULL;  /* start index for each state */
static int *degree     = NULL;  /* number of outgoing edges */

static seq_t *dp_cur  = NULL;   /* counts for current row length */
static seq_t *dp_next = NULL;   /* temporary buffer for next length */

static seq_t *cache    = NULL;   /* cache of a(n) for n ≥ 0 */
static size_t cache_cap = 0;     /* capacity of cache array */
static idx_t  max_cached = 0;    /* highest n for which cache[n] is valid */

static bool initialized = false;

/* --------------------------------------------------------------------- */
/* Build state table and adjacency graph                                 */
/* --------------------------------------------------------------------- */
static void init(void)
{
    /* ----- decode all row states ------------------------------------- */
    for (int i = 0; i < N_STATES; ++i) {
        int v = i;
        for (int c = WIDTH - 1; c >= 0; --c) {
            row_vals[i][c] = v % 3;
            v /= 3;
        }
    }

    /* ----- count outgoing edges --------------------------------------- */
    degree = (int *)malloc(N_STATES * sizeof(int));
    memset(degree, 0, N_STATES * sizeof(int));

    for (int s = 0; s < N_STATES; ++s) {
        for (int t = 0; t < N_STATES; ++t) {
            bool ok = true;
            for (int col = 0; col < BLOCKS; ++col) {
                int sum = row_vals[s][col]   + row_vals[s][col+1] +
                          row_vals[t][col]   + row_vals[t][col+1];
                if (sum == 0 || sum == 1 || sum == 7 || sum == 8) {
                    ok = false;
                    break;
                }
            }
            if (ok) degree[s]++;
        }
    }

    /* ----- allocate global adjacency array ---------------------------- */
    size_t total_edges = 0;
    for (int s = 0; s < N_STATES; ++s) total_edges += degree[s];
    adjacency = (int *)malloc(total_edges * sizeof(int));

    adj_start = (int *)malloc(N_STATES * sizeof(int));
    int pos = 0;
    for (int s = 0; s < N_STATES; ++s) {
        adj_start[s] = pos;
        pos += degree[s];
    }

    /* ----- fill adjacency --------------------------------------------- */
    for (int s = 0; s < N_STATES; ++s) {
        int *ptr = adjacency + adj_start[s];
        for (int t = 0; t < N_STATES; ++t) {
            bool ok = true;
            for (int col = 0; col < BLOCKS; ++col) {
                int sum = row_vals[s][col]   + row_vals[s][col+1] +
                          row_vals[t][col]   + row_vals[t][col+1];
                if (sum == 0 || sum == 1 || sum == 7 || sum == 8) {
                    ok = false;
                    break;
                }
            }
            if (ok) *ptr++ = t;
        }
    }

    /* ----- DP storage --------------------------------------------------- */
    dp_cur  = (seq_t *)malloc(N_STATES * sizeof(seq_t));
    dp_next = (seq_t *)malloc(N_STATES * sizeof(seq_t));

    /* initial vector: one way for each single row */
    for (int s = 0; s < N_STATES; ++s) dp_cur[s] = 1;
}

/* --------------------------------------------------------------------- */
/* Grow cache and extend DP to cover n --------------------------------- */
static void ensure_cache(idx_t n)
{
    if (n <= max_cached) return;

    /* grow cache array if necessary */
    if (cache_cap == 0) cache_cap = 16;
    while (cache_cap <= (size_t)n) cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));

    /* extend DP from max_cached to n */
    for (idx_t step = max_cached; step < n; ++step) {
        /* dp_next = 0 */
        memset(dp_next, 0, N_STATES * sizeof(seq_t));

        for (int s = 0; s < N_STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            int start = adj_start[s];
            int end   = start + degree[s];
            for (int idx = start; idx < end; ++idx) {
                int t = adjacency[idx];
                dp_next[t] += val;
            }
        }

        /* sum over all end states */
        seq_t sum = 0;
        for (int t = 0; t < N_STATES; ++t) sum += dp_next[t];

        cache[step + 1] = sum;          /* cache[1] corresponds to 2 rows */

        /* swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur   = dp_next;
        dp_next  = tmp;
    }
    max_cached = n;
}

/* --------------------------------------------------------------------- */
/* Public function                                                       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        init();
        initialized = true;
    }
    ensure_cache(n);
    return cache[n];
}
