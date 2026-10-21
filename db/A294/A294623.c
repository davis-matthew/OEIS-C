/*
 * A294623  Number of partitions of n into distinct generalized heptagonal
 *          numbers (A085787).
 * https://oeis.org/A294623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Product_{k>=1} (1 + x^{k*(5*k-3)/2})*(1 + x^{k*(5*k+3)/2})
 * Range:     u8 n <= 405, u16 n <= 1437, u32 n <= 6342, u64 n <= 9720,
 *            u128 n <= 9720, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of dp values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements currently stored (max_n+1) */
static size_t cache_cap = 0;   /* allocated size */

/* recompute cache up to new_max (inclusive) */
static void recompute(idx_t new_max)
{
    /* allocate enough space for cache */
    size_t new_cap = (size_t)new_max + 1;
    if (cache == NULL || cache_cap < new_cap) {
        free(cache);
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise cache: dp[0] = 1, others 0 */
    cache[0] = (seq_t)1;
    for (idx_t i = 1; i <= new_max; ++i)
        cache[i] = (seq_t)0;

    /* compute all distinct generalized heptagonal numbers <= new_max */
    /* we store them in a small static array */
    idx_t coins[256];
    size_t num_coins = 0;
    for (idx_t k = 1;; ++k) {
        idx_t h1 = k * (5 * k - 3) / 2;
        if (h1 <= new_max) {
            if (num_coins < 256) coins[num_coins++] = h1;
        }
        idx_t h2 = k * (5 * k + 3) / 2;
        if (h2 <= new_max) {
            if (num_coins < 256) coins[num_coins++] = h2;
        }
        if (h1 > new_max && h2 > new_max) break;
    }

    /* subset‑sum DP with each coin used at most once */
    for (size_t ci = 0; ci < num_coins; ++ci) {
        idx_t coin = coins[ci];
        for (idx_t s = new_max; s >= coin; --s)
            cache[s] += cache[s - coin];
    }

    cache_size = new_cap;
}

seq_t a(idx_t n)
{
    if (n >= cache_size)
        recompute(n);
    return cache[n];
}
