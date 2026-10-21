/*
 * A250712  Number of (5+1)X(n+1) 0..3 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (5+1)×(n+1) arrays with entries in {0,1,2,3}
 *            satisfying the two inequalities: 1) For each column j≥1 the sums
 *            s_i=x(i,j)+x(i,j-1) are nondecreasing in i; 2) For each row i,
 *            the sequence of min(x(i,j),x(i−1,j)) over j is nondecreasing
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 17, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* number of column states – 4⁶ = 4096 */
#define NUM_STATES   4096

/* maximum cache size we allow – can grow dynamically */
#define MAX_CACHE_N  20000

/* decode state index into its 6 digits (0..3) */
static inline void decode_state(int s, int v[6]) {
    for (int k = 5; k >= 0; --k) {
        v[k] = s & 3;
        s >>= 2;
    }
}

/* transition bookkeeping */
static int  trans_start[NUM_STATES];
static int  trans_count [NUM_STATES];
static int *trans_next;             /* flattened adjacency list */

/* DP buffers */
static seq_t state_buff[2][NUM_STATES];
static int   cur = 0;

/* sequence cache */
static seq_t *cache     = NULL;
static idx_t  cache_cap = 0;        /* allocated size (including index 0) */
static idx_t  max_cached_n = 0;

/* one‑time initialisation */
static void init(void) {
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* allocate space for worst‑case edges: all 4096 × 4096 pairs */
    int max_edges = NUM_STATES * NUM_STATES;
    trans_next = (int*)malloc(max_edges * sizeof(int));
    int edge_cnt = 0;

    int prev[6], curr[6], prev_min[5];
    for (int p = 0; p < NUM_STATES; ++p) {
        decode_state(p, prev);
        /* min of adjacent digits in the previous column */
        for (int i = 1; i < 6; ++i)
            prev_min[i-1] = (prev[i] < prev[i-1]) ? prev[i] : prev[i-1];

        trans_start[p] = edge_cnt;

        for (int q = 0; q < NUM_STATES; ++q) {
            decode_state(q, curr);

            int ok = 1;
            int s_prev = curr[0] + prev[0];
            for (int i = 1; i < 6 && ok; ++i) {
                int s = curr[i] + prev[i];
                if (s < s_prev) { ok = 0; break; }
                s_prev = s;

                int curr_min = (curr[i] < curr[i-1]) ? curr[i] : curr[i-1];
                if (curr_min < prev_min[i-1]) { ok = 0; break; }
            }
            if (ok) {
                trans_next[edge_cnt++] = q;
            }
        }
        trans_count[p] = edge_cnt - trans_start[p];
    }

    /* initialise DP for the first column: each state contributes 1 */
    for (int i = 0; i < NUM_STATES; ++i)
        state_buff[cur][i] = 1;

    /* allocate cache for sequence values */
    cache_cap = MAX_CACHE_N + 1;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache[0] = 0;    /* unused, but defined */
    max_cached_n = 0;
}

/* Tet function as required by the build */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return 0;   /* below offset – trivial */

    init();

    /* grow cache if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = (n + 1) * 2;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;      /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute DP up to requested n */
    for (idx_t step = max_cached_n + 1; step <= n; ++step) {
        seq_t *dp_cur  = state_buff[cur];
        seq_t *dp_next = state_buff[1 - cur];
        for (int i = 0; i < NUM_STATES; ++i) dp_next[i] = 0;

        /* transition */
        for (int p = 0; p < NUM_STATES; ++p) {
            seq_t val = dp_cur[p];
            if (val == 0) continue;
            int start = trans_start[p];
            int end   = start + trans_count[p];
            for (int t = start; t < end; ++t) {
                int q = trans_next[t];
                dp_next[q] = dp_next[q] + val;
            }
        }

        /* sum over all terminal states gives a(step) */
        seq_t sum = 0;
        for (int i = 0; i < NUM_STATES; ++i)
            sum = sum + dp_next[i];
        cache[step] = sum;

        cur = 1 - cur;   /* swap buffers for next iteration */
    }

    max_cached_n = n;
    return cache[n];
}
