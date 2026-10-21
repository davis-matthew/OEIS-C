/*
 * A251500  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 3 4 5 6 7 8 or 9.
 * https://oeis.org/A251500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251500: Number of (n+1)×2 arrays over {0..3} with every 2×2
   subblock summing to 3,4,5,6,7,8 or 9.  
   Calculation by dynamic programming on the 16 possible column states.
   a(n) is the sum of all length‑n walks through the state transition graph,
   with each walk corresponding to a valid array.  */
   

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* adjacency between 16 states (x1,y1) -> (x2,y2) is allowed iff
   x1 + y1 + x2 + y2 is between 3 and 9 inclusive. */
static bool trans[16][16];
static bool adj_ready = false;

static void init_adj(void)
{
    if (adj_ready) return;
    for (int s = 0; s < 16; ++s) {
        int s0 = s >> 2;          /* first column value   */
        int s1 = s & 3;           /* second column value  */
        for (int t = 0; t < 16; ++t) {
            int t0 = t >> 2;
            int t1 = t & 3;
            int sum = s0 + s1 + t0 + t1;
            trans[s][t] = (sum >= 3 && sum <= 9);
        }
    }
    adj_ready = true;
}

/* Cache for a(n), indexed from 1.  0 is unused. */
static seq_t *a_cache = NULL;
static size_t cache_cap = 0;      /* number of elements that can be stored */
static size_t cached_n = 0;       /* highest n for which a_cache[n] is filled */

/* Previous and current state distributions for the DP. */
static seq_t dp_prev[16];
static seq_t dp_curr[16];
static bool dp_inited = false;

/* Make sure the cache can hold indices up to 'needed' (inclusive) */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = needed > 16 ? needed * 2 : 32;
    seq_t *new_buf = (seq_t*)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_buf) exit(EXIT_FAILURE);
    if (a_cache) {
        memcpy(new_buf, a_cache, (cache_cap + 1) * sizeof(seq_t));
        free(a_cache);
    }
    memset(new_buf, 0, (new_cap + 1) * sizeof(seq_t));
    a_cache = new_buf;
    cache_cap = new_cap;
}

/* Compute terms up to 'target', storing them in the cache. */
static void compute_up_to(idx_t target)
{
    if (target == 0 || target <= cached_n) return;

    init_adj();

    if (!dp_inited) {
        /* dp_prev represents walks of length 0 ending in each state (1 for all). */
        for (int i = 0; i < 16; ++i) dp_prev[i] = (seq_t)1;
        dp_inited = true;
    }

    for (idx_t n = cached_n + 1; n <= target; ++n) {
        memset(dp_curr, 0, sizeof(dp_curr));

        for (int s = 0; s < 16; ++s) {
            seq_t cnt = dp_prev[s];
            if (!cnt) continue;
            for (int t = 0; t < 16; ++t) {
                if (trans[s][t]) {
                    dp_curr[t] += cnt;
                }
            }
        }

        /* Sum all walks of length n. */
        seq_t term = 0;
        for (int t = 0; t < 16; ++t) term += dp_curr[t];

        ensure_cache((size_t)n);
        a_cache[(size_t)n] = term;

        /* Prepare for next iteration. */
        memcpy(dp_prev, dp_curr, sizeof(dp_prev));
    }
    cached_n = (size_t)target;
}

/* Public interface demanded by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but never used in OEIS offset */
    compute_up_to(n);
    return a_cache[(size_t)n];
}
