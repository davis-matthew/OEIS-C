/*
 * A251215  Number of (n+1)X(4+1) 0..1 arrays with no 2X2 subblock having zero
 *          or two 1s.
 * https://oeis.org/A251215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts (n+1)×5 binary arrays with no 2×2 subblock having 0
 *            or 2 ones. The computation uses a 32‑state DFA: a state is a row
 *            (5 bits); transitions are allowed if the two consecutive rows
 *            satisfy the local constraint on each 2×2 subblock. The number of
 *            length‑L sequences is the sum of the coun...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static char M[32][32];          /* adjacency matrix (0/1) */
static bool prepared = false;   /* matrix & init state prepared? */

/* Dynamic cache of a(n) values.  cache[0] is unused. */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;    /* maximum index + 1 for cache */

/* Global DP vectors for current and next state counts. */
static seq_t dp[32];
static seq_t dp_next[32];

/* Ensure the cache has space for indices up to 'needed'. */
static void ensure_cache_size(idx_t needed)
{
    if (needed < cache_cap)
        return;
    size_t new_cap = (size_t)needed + 1;
    seq_t *newc = seq_alloc_new_array(new_cap);
    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i)
            newc[i] = cache[i];
        free_seq_array(cache);
    }
    cache = newc;
    cache_cap = new_cap;
}

/* Prepare adjacency matrix and initial DP state. */
static void prepare(void)
{
    if (prepared)
        return;
    /* adjacency: M[p][q] = 1 if rows p->q are compatible */
    for (int p = 0; p < 32; ++p) {
        for (int q = 0; q < 32; ++q) {
            bool good = true;
            for (int k = 0; k < 4; ++k) {      /* 4 overlapping 2×2 blocks */
                int s = ((p >> k) & 1) + ((p >> (k + 1)) & 1)
                      + ((q >> k) & 1) + ((q >> (k + 1)) & 1);
                if (s == 0 || s == 2) {
                    good = false;
                    break;
                }
            }
            M[p][q] = good ? 1 : 0;
        }
    }

    /* initialise DP vector: one way to start any of the 32 rows */
    for (int i = 0; i < 32; ++i)
        dp[i] = (seq_t)1;

    /* initialise cache */
    cache_cap = 2;          /* at least indices 0 and 1 */
    cache = seq_alloc_new_array(cache_cap);
    cache[0] = (seq_t)0;    /* unused */
    prepared = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n=0, return 0 */

    prepare();

    /* current row length (number of rows processed) */
    static idx_t cur_len = 1;     /* starts with length 1 (single row) */

    /* extend cache and DP until we have a(n) */
    while (cur_len - 1 < n) {
        /* compute dp_next for next row length (cur_len + 1) */
        for (int j = 0; j < 32; ++j)
            dp_next[j] = (seq_t)0;
        for (int i = 0; i < 32; ++i) {
            if (dp[i] == 0)
                continue;
            for (int j = 0; j < 32; ++j) {
                if (M[i][j])
                    dp_next[j] += dp[i];
            }
        }

        /* total sequences for length cur_len + 1 -> n = cur_len */
        seq_t total = (seq_t)0;
        for (int j = 0; j < 32; ++j)
            total += dp_next[j];

        /* store a(cur_len) */
        if (cur_len >= cache_cap)
            ensure_cache_size(cur_len + 1);
        cache[cur_len] = total;

        /* roll DP */
        for (int j = 0; j < 32; ++j)
            dp[j] = dp_next[j];
        ++cur_len;
    }

    return cache[n];
}
