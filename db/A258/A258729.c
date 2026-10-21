/*
 * A258729  Number of length n+7 0..3 arrays with at most one downstep in
 *          every 7 consecutive neighbor pairs.
 * https://oeis.org/A258729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 17, u64 n <= 45, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) counts the number of length n+7 arrays over {0,1,2,3}
   with at most one downstep (a[i]>a[i+1]) in every consecutive 7
   neighbor pairs. Computed via dynamic programming over the last 7
   elements. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STATES (1 << 14)   /* 4^7 = 16384 */

static unsigned char s_last[STATES];          /* last element of the state */
static unsigned char s_down[STATES];          /* number of downsteps in the 6 internal pairs */
static int16_t      s_next[STATES][4];        /* dest state or -1 if forbidden */

static seq_t *dp_cur = NULL;                 /* DP vector for current length */
static seq_t *dp_next = NULL;                /* DP vector for next length */
static seq_t *cache = NULL;                  /* a(1..max_n) cache */
static size_t cache_len = 0;                 /* allocated cache size */
static idx_t  current_n = 0;                 /* largest n for which cache[n] is valid */
static bool initialized = false;

/* Pre‑compute state tables and initialise DP vectors. */
static void init_state(void)
{
    for (int s = 0; s < STATES; ++s) {
        int t = s;
        unsigned char d[7];
        for (int i = 0; i < 7; ++i) {
            d[i] = t & 3;
            t >>= 2;
        }
        s_last[s] = d[6];
        unsigned char cnt = 0;
        for (int i = 0; i < 6; ++i)
            if (d[i] > d[i+1]) ++cnt;
        s_down[s] = cnt;

        for (int v = 0; v < 4; ++v) {
            int newdown = (d[6] > v) ? 1 : 0;
            if (cnt + newdown <= 1) {
                s_next[s][v] = (int16_t)((s >> 2) | (v << 12));
            } else {
                s_next[s][v] = -1;
            }
        }
    }

    dp_cur  = (seq_t*)malloc(sizeof(seq_t) * STATES);
    dp_next = (seq_t*)malloc(sizeof(seq_t) * STATES);
    for (int s = 0; s < STATES; ++s)
        dp_cur[s] = 1;             /* one sequence of length 7 for each state */
}

/* Main function requested by the problem. */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_state();
        initialized = true;
    }
    if (n == 0) return 0;                 /* not defined; placeholder */

    if ((idx_t)cache_len <= n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        /* initialise newly allocated part to zero */
        for (size_t i = cache_len; i < new_len; ++i) tmp[i] = 0;
        cache     = tmp;
        cache_len = new_len;
    }

    if (n <= current_n) return cache[n];

    /* Extend DP to compute a(current_n+1) … a(n) */
    for (idx_t step = current_n + 1; step <= n; ++step) {
        memset(dp_next, 0, sizeof(seq_t) * STATES);

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            for (int v = 0; v < 4; ++v) {
                int dest = s_next[s][v];
                if (dest < 0) continue;
                dp_next[dest] += val;
            }
        }

        /* Sum over all states to obtain a(step) */
        seq_t total = 0;
        for (int s = 0; s < STATES; ++s) total += dp_next[s];
        cache[step] = total;

        /* Prepare for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;
        current_n = step;
    }
    return cache[n];
}
