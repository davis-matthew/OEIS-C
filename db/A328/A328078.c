/*
 * A328078  Number of regions after n generations of Jim Conant's iterative
 *          dissection of a square.
 * https://oeis.org/A328078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 6*a(n-3) + 7*a(n-4) - 4*a(n-5) -
 *            12*a(n-6) + 20*a(n-7) - 12*a(n-8) + 12*a(n-10) - 16*a(n-11) +
 *            8*a(n-12) for n>12 Initial terms: a(0)=1, a(1)=2, a(2)=3,
 *            a(3)=5, a(4)=9, a(5)=15, a(6)=27, a(7)=48, a(8)=91, a(9)=169,
 *            a(10)=325, a(11)=618, a(12)=1201
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Initial 13 terms stored as constants */
static const seq_t base_vals[13] = {
    (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)9,
    (seq_t)15, (seq_t)27, (seq_t)48, (seq_t)91, (seq_t)169,
    (seq_t)325, (seq_t)618, (seq_t)1201
};

/* Static cache for computed values */
static seq_t *cache        = NULL;    /* pointer to cache array          */
static size_t cache_size   = 0;       /* number of terms currently stored */
static size_t cache_cap    = 0;       /* allocated capacity of the cache */

/* Ensure that the cache contains all terms up to n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        cache_cap = needed > 13 ? needed : 13;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return;            /* allocation failed – leave cache NULL */
        for (size_t i = 0; i < 13; ++i) {
            cache[i] = base_vals[i];
        }
        cache_size = 13;
    } else if (cache_cap < needed) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }   /* if realloc fails, keep the old cache */
    }

    /* Compute missing terms */
    if (cache_size < needed) {
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            size_t idx = (size_t)i;
            seq_t v = cache[idx-1] * 2
                    + cache[idx-2] * 2
                    - cache[idx-3] * 6
                    + cache[idx-4] * 7
                    - cache[idx-5] * 4
                    - cache[idx-6] * 12
                    + cache[idx-7] * 20
                    - cache[idx-8] * 12
                    + cache[idx-10] * 12
                    - cache[idx-11] * 16
                    + cache[idx-12] * 8;
            cache[idx] = v;
        }
        cache_size = needed;
    }
}

seq_t a(idx_t n)
{
    if (n < 13) return base_vals[n];
    ensure_cache(n);
    return cache[(size_t)n];
}
