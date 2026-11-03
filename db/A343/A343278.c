/*
 * A343278  a(n) = Stirling2(n, ceiling(n/2)).
 * https://oeis.org/A343278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Stirling2(n, ceil(n/2)) – number of set partitions of an
 *            n‑set into * ceil(n/2) blocks. Implementation uses a dynamic
 *            row‑by‑row DP for the Stirling * numbers of the second kind,
 *            memoising results so that multiple calls are * efficient
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static seq_t *dp      = NULL;   /* dp[k] holds S(current_n, k) during computation */
static size_t  dp_cap = 0;      /* current capacity of dp array             */

static seq_t *cache      = NULL;   /* cache[n] = a(n)                         */
static size_t cache_cap = 0;      /* current capacity of cache array           */

static idx_t max_computed = 0;     /* highest n that has already been computed */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) = 1 */
        return (seq_t)1;
    }

    /* make sure cache has enough space for up to n */
    if (cache_cap < (size_t)(n + 1)) {
        grow_seq_array(&cache, &cache_cap, (size_t)(n + 1));
    }

    if (dp_cap < (size_t)(n + 1)) {
        grow_seq_array(&dp, &dp_cap, (size_t)(n + 1));
    }

    /* initialise for the very first call */
    if (max_computed == 0) {
        /* dp[0] = S(0,0) = 1; all other dp[k] start at 0 */
        for (size_t i = 0; i <= n; ++i) {
            dp[i] = (seq_t)0;
        }
        dp[0] = (seq_t)1;
        cache[0] = (seq_t)1;
    }

    /* fill values up to n if necessary */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        /* compute row cur using one‑dimensional DP from high to low */
        for (idx_t k = cur; k >= 1; --k) {
            /* dp[k] = dp[k-1] + k * dp[k] (old dp[k] is S(cur-1,k)) */
            dp[k] = dp[k-1] + (seq_t)k * dp[k];
        }
        dp[0] = (seq_t)0;  /* S(cur,0) = 0 for cur > 0 */

        /* store a(cur) = dp[ceil(cur/2)] */
        idx_t k = (cur + 1) / 2;  /* ceil(cur/2) */
        cache[cur] = dp[k];
    }

    max_computed = n;
    return cache[n];
}
