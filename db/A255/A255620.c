/*
 * A255620  Number of length n+6 0..2 arrays with at most two downsteps in
 *          every 6 consecutive neighbor pairs.
 * https://oeis.org/A255620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence counts 0‒2 arrays of length n+6 in which every 7
 *            consecutive terms contain at most two positions i with a[i] >
 *            a[i+1]. We compute it by a sliding‑window DP over the last six
 *            symbols
 * Range:     u8 none, u16 n <= 4, u32 n <= 16, u64 n <= 38, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, realloc */

#define STATE_COUNT 729            /* 3^6 */
#define STATE_FIVE 243             /* 3^5 */
#define MAX_COMPUTE 10000          /* sufficient for the test limits */

static int      down_in[STATE_COUNT];   /* down counts inside state (5 edges)   */
static int      last_digit[STATE_COUNT];/* last symbol of the 6‑symbol state   */
static seq_t    dp[STATE_COUNT];        /* DP for current length              */
static seq_t    next_dp[STATE_COUNT];   /* temporary DP for next length       */
static seq_t*   cache = NULL;           /* cache of a(n), 1‑based             */
static size_t   cache_cap = 0;          /* allocated size of cache            */
static size_t   computed_len = 0;       /* greatest n with cached value       */
static bool     initialized = false;    /* one‑time init flag                 */

/* ------------------------------------------------------------------ */
/* Initialise transition data and cache up to MAX_COMPUTE              */
/* ------------------------------------------------------------------ */
static void init(void)
{
    if (initialized) return;

    /* Pre‑compute number of downsteps inside each state and the last
       digit of the state (needed for transition). */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int digits[6];
        int t = s;
        for (int k = 5; k >= 0; --k) {
            digits[k] = t % 3;
            t /= 3;
        }
        int count = 0;
        for (int i = 0; i < 5; ++i) {
            if (digits[i] > digits[i + 1]) ++count;
        }
        down_in[s]   = count;
        last_digit[s] = digits[5];
    }

    /* Start with all 6‑length suffixes allowed. */
    for (int s = 0; s < STATE_COUNT; ++s) dp[s] = 1;

    /* Allocate cache for n = 1 … MAX_COMPUTE. */
    cache_cap = MAX_COMPUTE + 1;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));

    /* Compute a(n) for n = 1 … MAX_COMPUTE. */
    for (size_t n = 1; n <= MAX_COMPUTE; ++n) {
        /* Zero next_dp. */
        for (int s = 0; s < STATE_COUNT; ++s) next_dp[s] = 0;

        /* Transition: add a new symbol to the suffix. */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cur = dp[s];
            if (!cur) continue;
            for (int nx = 0; nx < 3; ++nx) {
                if (down_in[s] + ((last_digit[s] > nx) ? 1 : 0) <= 2) {
                    int ns = ((s % STATE_FIVE) * 3) + nx;
                    next_dp[ns] += cur;
                }
            }
        }

        /* Update dp to next length. */
        for (int s = 0; s < STATE_COUNT; ++s) dp[s] = next_dp[s];

        /* Sum all states to obtain a(n). */
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s) total += dp[s];
        cache[n] = total;
    }

    computed_len = MAX_COMPUTE;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Public function required by the harness.                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* not part of the OEIS sequence */

    init();                  /* ensure tables and cache are ready */

    /* Return cached value if already computed. */
    if ((size_t)n <= computed_len) return cache[n];

    /* Extend computation up to requested n. */
    for (size_t cur_n = computed_len + 1; cur_n <= (size_t)n; ++cur_n) {
        /* Zero next_dp. */
        for (int s = 0; s < STATE_COUNT; ++s) next_dp[s] = 0;

        /* Transition as before. */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cur = dp[s];
            if (!cur) continue;
            for (int nx = 0; nx < 3; ++nx) {
                if (down_in[s] + ((last_digit[s] > nx) ? 1 : 0) <= 2) {
                    int ns = ((s % STATE_FIVE) * 3) + nx;
                    next_dp[ns] += cur;
                }
            }
        }

        /* Update dp. */
        for (int s = 0; s < STATE_COUNT; ++s) dp[s] = next_dp[s];

        /* Sum to get a(cur_n). */
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s) total += dp[s];

        /* Resize cache if necessary. */
        if (cur_n + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < cur_n + 1) new_cap = cur_n + 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cur_n] = total;
    }

    computed_len = n;
    return cache[n];
}
