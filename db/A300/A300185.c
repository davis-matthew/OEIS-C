/*
 * A300185  Irregular triangle read by rows: T(n, {j,k}) is the number of
 *          partitions of n that have exactly j parts equal to k; 1 <= j <= n,
 *          1 <= k <= n.
 * https://oeis.org/A300185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth entry of the irregular triangle read by rows *
 *            (OEIS A300185). For each row r the values are produced by
 *            iterating * k = 1..r, and for each k by j = 1..r. The entry
 *            T(r,{j,k}) equals the * number of partitions of r that contain
 *            exactly j parts equal to k. * This can be computed by...
 * Range:     u8 n <= 3355, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* ----- memoisation storage ----- */
static seq_t *cache = NULL;      /* 0‑based storage of a(1), a(2), … */
static size_t cache_len = 0;    /* number of values stored */
static size_t cache_cap = 0;    /* current allocation capacity */
static idx_t last_row = 0;      /* last row that has been generated */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return;                  /* allocation failure – caller will return 0 */
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate a complete row 'r' and append its r² entries to the cache.
 * The values are emitted in the order k = 1..r, j = 1..r. */
static void generate_row(idx_t r)
{
    for (idx_t k = 1; k <= r; ++k) {
        /* dp[m] = number of partitions of m that use parts ≠ k */
        seq_t *dp = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
        if (!dp)
            return;  /* unlikely for small r, but abort on failure */
        dp[0] = 1;
        for (idx_t s = 1; s <= r; ++s) {
            if (s == k)
                continue;
            for (idx_t m = s; m <= r; ++m) {
                dp[m] += dp[m - s];
            }
        }
        /* Emit the r values for this k */
        for (idx_t j = 1; j <= r; ++j) {
            seq_t val = 0;
            if (j * k <= r) {
                idx_t rem = r - j * k;
                val = dp[rem];
            }
            ensure_cache(cache_len + 1);
            cache[cache_len++] = val;
        }
        free(dp);
    }
    last_row = r;
}

/* External interface required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is 1‑based; 0 is undefined */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        generate_row(last_row + 1);
        if (cache_len == 0)          /* allocation failure – give up */
            return 0;
    }
    return cache[n - 1];
}
