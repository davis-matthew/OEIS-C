/*
 * A311285  Coordination sequence Gal.4.70.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is defined by the recurrence (valid for n > 10):
 *   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 3*a(n-4) + 4*a(n-5)
 *          - 3*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10)
 * Initial values:
 *   a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=16,
 *   a(5)=21, a(6)=26, a(7)=30, a(8)=34, a(9)=37, a(10)=40
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static state for memoisation */
static seq_t *cache = NULL;        /* cached values */
static size_t cache_len = 0;       /* number of values currently stored */
static size_t cache_cap = 0;       /* allocated capacity for cache */

/* Initial 11 terms */
static const seq_t init_seq[11] = {
    1, 4, 8, 12, 16,
    21, 26, 30, 34, 37, 40
};

/* Expand cache so that it can hold 'needed_cap' elements */
static void ensure_capacity(size_t needed_cap)
{
    if (cache_cap >= needed_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed_cap)
        new_cap <<= 1;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* In case of allocation failure, abort – the calling environment
         * must handle this appropriately. */
        abort();
    }

    if (cache) {
        memcpy(new_arr, cache, cache_len * sizeof(seq_t));
        free(cache);
    }

    cache = new_arr;
    cache_cap = new_cap;
}

/* Compute a(n) for all n up to 'target' (inclusive) */
static void compute_upto(idx_t target)
{
    size_t t = (size_t)target;

    /* Initialise cache on first use */
    if (cache_len == 0) {
        ensure_capacity(11);
        memcpy(cache, init_seq, 11 * sizeof(seq_t));
        cache_len = 11;
    }

    if (t >= cache_cap)
        ensure_capacity(t + 1);

    for (size_t i = cache_len; i <= t; ++i) {
        long long val =
            2LL * (long long)cache[i - 1]
          - 2LL * (long long)cache[i - 2]
          + 2LL * (long long)cache[i - 3]
          - 3LL * (long long)cache[i - 4]
          + 4LL * (long long)cache[i - 5]
          - 3LL * (long long)cache[i - 6]
          + 2LL * (long long)cache[i - 7]
          - 2LL * (long long)cache[i - 8]
          + 2LL * (long long)cache[i - 9]
          - (long long)cache[i - 10];
        cache[i] = (seq_t)val;
    }

    cache_len = t + 1;
}

/* Public accessor */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    if (idx < 11)
        return init_seq[idx];

    if (idx >= cache_len)
        compute_upto(n);

    return cache[idx];
}
