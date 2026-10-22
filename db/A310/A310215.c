/*
 * A310215  Coordination sequence Gal.3.2.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-4) - a(n-5) + a(n-6) -
 *            a(n-7) Initial values: a(0)=1, a(1)=3, a(2)=7, a(3)=12, a(4)=14,
 *            a(5)=17, a(6)=22, a(7)=27
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cached values a(0)..a(k) */
static size_t cache_len = 0;    /* number of cached values */
static size_t cache_cap = 0;    /* current allocation size */

/* Ensure cache contains a(0)..a(n). */
static void
ensure_cache(idx_t n)
{
    /* Initial seed values */
    static const seq_t init_vals[8] = { (seq_t)1, (seq_t)3, (seq_t)7,
                                       (seq_t)12, (seq_t)14, (seq_t)17,
                                       (seq_t)22, (seq_t)27 };

    /* Allocate the initial 8 entries if needed */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* allocation failure */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
        cache_len = 8;
    }

    /* Grow the cache to include up to n (if necessary) */
    if ((idx_t)cache_len <= n) {
        idx_t new_cap_idx = n + 1;          /* need indices [0..n] inclusive */
        size_t new_cap = (size_t)new_cap_idx;
        if (new_cap > cache_cap) {
            void *tmp = realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();              /* allocation failure */
            cache = (seq_t *)tmp;
            cache_cap = new_cap;
        }

        /* Compute new terms iteratively */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            if (i <= 7) {
                /* already set from init_vals */
                continue;
            }
            /* Compute using the recurrence:
               a[i] = a[i-1] - a[i-2] + a[i-3] + a[i-4]
                      - a[i-5] + a[i-6] - a[i-7]  */
            seq_t sum1 = cache[i-1] + cache[i-3] + cache[i-4] + cache[i-6];
            seq_t sum2 = cache[i-2] + cache[i-5] + cache[i-7];
            cache[i] = sum1 - sum2;          /* sum1 >= sum2 for all i */
        }
        cache_len = new_cap_idx;
    }
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
