/*
 * A250012  Number of length n+5 0..6 arrays with no six consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 arrays over {0,…,6} for which for
 *            every six consecutive terms, the maximum of any three terms is
 *            never equal to the minimum of the remaining three terms. The
 *            algorithm uses a transfer‑matrix DP over the 5‑suffix of the
 *            current array
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 20, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHA          7U          /* alphabet 0..6 */
#define STATE_COUNT   16807U        /* ALPHA^5 */

/* Transition tables */
static uint16_t *nxt_state = NULL;   /* STATE_COUNT*ALPHA entries */
static uint8_t  *allowed   = NULL;   /* STATE_COUNT*ALPHA entries */

/* DP arrays for the current and next step */
static seq_t *dp     = NULL;
static seq_t *nextdp = NULL;

/* Cache of sequence values a(1), a(2), ... (1‑based index) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;          /* number of cached values + 1,
                                         cache[0] is unused, cache_len==1
                                         means no value stored yet */

/* length represented by dp (initially 5) */
static int cur_len = 5;
static int init_done = 0;

/* Pre‑computed combinations of 3 indices out of 6 */
static int combs[20][3];
static int comps[20][3];
static void init_combos(void)
{
    int idx = 0;
    for (int i = 0; i < 6; ++i)
        for (int j = i + 1; j < 6; ++j)
            for (int k = j + 1; k < 6; ++k) {
                combs[idx][0] = i; combs[idx][1] = j; combs[idx][2] = k;
                int ci = 0;
                for (int c = 0; c < 6; ++c)
                    if (c != i && c != j && c != k)
                        comps[idx][ci++] = c;
                ++idx;
            }
}

/* Build the transition tables and initialize the DP state */
static void init_transitions(void)
{
    if (init_done) return;
    init_done = 1;

    /* Allocate tables */
    nxt_state = (uint16_t*)malloc((size_t)STATE_COUNT * ALPHA * sizeof(uint16_t));
    allowed   = (uint8_t*)malloc((size_t)STATE_COUNT * ALPHA * sizeof(uint8_t));
    dp        = (seq_t*)malloc((size_t)STATE_COUNT * sizeof(seq_t));
    nextdp    = (seq_t*)malloc((size_t)STATE_COUNT * sizeof(seq_t));

    init_combos();

    /* Pre‑compute allowed transitions */
    for (int idx = 0; idx < STATE_COUNT; ++idx) {
        /* Decode the 5‑tuple for this state */
        int tmp = idx;
        int d[5];
        /* d[0] is the most significant digit */
        for (int k = 4; k >= 0; --k) {
            d[k] = tmp % ALPHA;
            tmp /= ALPHA;
        }

        for (int v = 0; v < ALPHA; ++v) {
            int w[6] = { d[0], d[1], d[2], d[3], d[4], v };
            bool ok = true;
            for (int j = 0; j < 20 && ok; ++j) {
                /* Compute max of the chosen triple */
                int maxval = w[combs[j][0]];
                if (w[combs[j][1]] > maxval) maxval = w[combs[j][1]];
                if (w[combs[j][2]] > maxval) maxval = w[combs[j][2]];
                /* Compute min of the complementary triple */
                int minval = w[comps[j][0]];
                if (w[comps[j][1]] < minval) minval = w[comps[j][1]];
                if (w[comps[j][2]] < minval) minval = w[comps[j][2]];
                if (maxval == minval) ok = false;
            }
            allowed[idx * ALPHA + v] = ok ? 1 : 0;

            /* Compute the next state (drop d[0], shift left, append v) */
            int nidx = (((d[1] * ALPHA + d[2]) * ALPHA + d[3]) * ALPHA + d[4]) * ALPHA + v;
            nxt_state[idx * ALPHA + v] = (uint16_t)nidx;
        }
    }

    /* Initialize DP for length 5: every 5‑tuple is a valid partial sequence */
    for (int i = 0; i < (int)STATE_COUNT; ++i)
        dp[i] = (seq_t)1;

    /* Prepare cache: cache[0] unused, cache_len==1 means nothing cached yet */
    cache_cap = 8;
    cache_len = 1;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache[0] = 0;
}

/* Extend the DP computation up to the required n */
static void compute_up_to(idx_t n)
{
    init_transitions();
    int target_len = (int)n + 5;   /* actual array length */

    while (cur_len < target_len) {
        /* zero nextdp */
        memset(nextdp, 0, (size_t)STATE_COUNT * sizeof(seq_t));

        /* transition */
        for (int s = 0; s < (int)STATE_COUNT; ++s) {
            seq_t val = dp[s];
            if (!val) continue;
            uint8_t  *allowed_row = &allowed[s * ALPHA];
            uint16_t *next_row    = &nxt_state[s * ALPHA];
            for (int v = 0; v < ALPHA; ++v) {
                if (!allowed_row[v]) continue;
                nextdp[next_row[v]] += val;
            }
        }

        /* total number of valid arrays of current length */
        seq_t total = 0;
        for (int i = 0; i < (int)STATE_COUNT; ++i)
            total += nextdp[i];

        /* cache the result for a( cur_len - 4 ) */
        if (cache_len >= cache_cap) {
            cache_cap = (cache_cap << 1) + 1;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = total;
        ++cache_len;

        /* prepare for next iteration */
        seq_t *tmp = dp;
        dp = nextdp;
        nextdp = tmp;
        ++cur_len;
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    compute_up_to(n);
    return cache[(size_t)n];
}
