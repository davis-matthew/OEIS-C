/*
 * A380292  Lagged Fibonacci Generator: a(n) = (100003 - 200003*n +
 *          300007*n^3) mod 10^6 for n <= 55, a(n) = (a(n-24) + a(n-55)) mod
 *          10^6.
 * https://oeis.org/A380292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (100003 - 200003*n + 300007*n^3) mod 10^6 for n <= 55,
 *            a(n) = (a(n-24) + a(n-55)) mod 10^6 for n > 55. Initial values
 *            (n = 1..24): 200007, 100053, 600183, 500439, 600863, 701497,
 *            602383, 103563, 5079, 106973, 209287, 112063, 615343, 519169,
 *            623583, 728627, 634343, 140773, 47959, 155943, 26...
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 1‑based cache array: cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cached_len = 0;  /* highest index that is valid */
static size_t cache_cap = 0;   /* allocated size */

/* Grow the cache so that it can hold indices up to n (1‑based). */
static void
ensure_cache(size_t n)
{
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();   /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Compute the nth term of the Lagged Fibonacci Generator. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* value below the offset is irrelevant */

    /* Ensure cache can accommodate n. */
    ensure_cache((size_t)n);

    /* Generate any required terms up to n. */
    for (idx_t i = (idx_t)(cached_len + 1); i <= n; ++i) {
        if (i <= 55) {
            /* Direct formula.  Use 64‑bit arithmetic for safety. */
            int64_t ii = (int64_t)i;
            int64_t val = 100003 - 200003 * ii + 300007 * ii * ii * ii;
            int64_t mod = 1000000;
            val %= mod;
            if (val < 0) val += mod;
            cache[i] = (seq_t)val;
        } else {
            /* Recurrence with modular reduction. */
            seq_t sum = cache[i - 24] + cache[i - 55];
            seq_t mod = (seq_t)1000000;
            if (sum >= mod) sum -= mod;   /* one subtraction suffices */
            cache[i] = sum;
        }
    }

    cached_len = (size_t)n;
    return cache[(size_t)n];
}
