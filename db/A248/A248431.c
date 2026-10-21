/*
 * A248431  Number of length n+2 0..6 arrays with every three consecutive
 *          terms having the sum of some two elements equal to twice the
 *          third.
 * https://oeis.org/A248431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-3) - 11*a(n-6) + 4*a(n-9) (empirical recurrence,
 *            using initial values a(1)=61, a(2)=105, a(3)=185) The following
 *            implementation uses dynamic programming on the last two digits
 *            of the array. It is fully index‑independent and memoises results
 *            for arbitrary n. All operations on seq_t ar...
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 30, u64 n <= 66, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Transition tables: 49 = 7*7 possible last‑two pairs. */
static int transition_nexts[49][7];
static int transition_nextcnt[49];
static bool transitions_ready = false;

static void prepare_transitions(void)
{
    if (transitions_ready) return;
    for (int x = 0; x <= 6; ++x) {
        for (int y = 0; y <= 6; ++y) {
            int idx = x * 7 + y;
            transition_nextcnt[idx] = 0;
            for (int z = 0; z <= 6; ++z) {
                bool ok =
                    (x + y == 2 * z) ||
                    (x + z == 2 * y) ||
                    (y + z == 2 * x);
                if (ok) {
                    transition_nexts[idx][transition_nextcnt[idx]++] = y * 7 + z;
                }
            }
        }
    }
    transitions_ready = true;
}

/* Memoisation cache for computed values a(1) … a(n). */
static seq_t *cache      = NULL;   /* cache[n] holds a(n) for n>=1   */
static size_t cache_cap  = 0;       /* allocated length of cache     */
static idx_t  max_n_done = 0;       /* largest n for which cache is valid */

/* DP state arrays for the current length (pair counts). */
static seq_t cur[49];
static seq_t nxt[49];

static void reallocate_cache_if_needed(idx_t n)
{
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* In a full implementation you might abort or return an error. */
            return;
        }
        /* Copy existing values. */
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        /* Zero new slots. */
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = (seq_t)0;
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Initialise the DP state for an array of length 2 (all 49 pairs). */
static void init_cur_from_pairs(void)
{
    for (int i = 0; i < 49; ++i) cur[i] = (seq_t)1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n >= 1 by problem statement, but we tolerate n == 0 gracefully. */
    if (n == 0) return (seq_t)49;  /* array of length 2: all 49 pairs */

    prepare_transitions();

    /* Allocate or grow the cache. */
    reallocate_cache_if_needed(n);

    /* Initialise state only on first use. */
    if (max_n_done == 0) {
        init_cur_from_pairs();
    }

    /* L_current corresponds to length of array represented by cur (which holds
       counts for length = L_current).  Since a(k) counts length k+2 arrays,
       cur corresponds to length = max_n_done + 2. */
    idx_t L_current = max_n_done + 2;  /* current array length */
    idx_t L_target  = n + 2;           /* desired array length */

    for (idx_t L = L_current + 1; L <= L_target; ++L) {
        /* Zero nxt. */
        for (int i = 0; i < 49; ++i) nxt[i] = (seq_t)0;
        /* Propagate one step. */
        for (int idx1 = 0; idx1 < 49; ++idx1) {
            seq_t val = cur[idx1];
            if (val == (seq_t)0) continue;
            int cnt = transition_nextcnt[idx1];
            for (int k = 0; k < cnt; ++k) {
                int idx2 = transition_nexts[idx1][k];
                nxt[idx2] += val;
            }
        }
        /* Sum all states to get a(L-2). */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 49; ++i) sum += nxt[i];
        cache[L - 2] = sum;   /* L-2 is the n value for this length */
        /* Prepare for next iteration. */
        for (int i = 0; i < 49; ++i) cur[i] = nxt[i];
    }

    max_n_done = n;
    return cache[n];
}
