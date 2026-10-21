/*
 * A289914  Coefficients of 1/(Sum_{k>=0} round((k+1)*r)(-x)^k), where r =
 *          7/5.
 * https://oeis.org/A289914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - a(n-4) (n>=7) Initial
 *            terms: a(0)=1, a(1)=3, a(2)=5, a(3)=9, a(4)=18, a(5)=35, a(6)=66
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 34, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache     = NULL;   /* stored terms          */
static size_t cache_cap = 0;      /* capacity in elements  */
static size_t cache_len = 0;      /* number of computed items */

/* Ensure cache can hold indices 0..n */
static void ensure_cache_alloc(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)18;
    if (n == 5) return (seq_t)35;
    if (n == 6) return (seq_t)66;

    ensure_cache_alloc(n);

    /* Initialise the cache with the first 7 known values */
    if (cache_len == 0) {
        size_t init = 7;
        if (cache_cap < init) {
            seq_t *tmp = (seq_t *)realloc(cache, init * sizeof(seq_t));
            cache = tmp;
            cache_cap = init;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)18;
        cache[5] = (seq_t)35;
        cache[6] = (seq_t)66;
        cache_len = 7;
    }

    /* If request already computed, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute missing values up to n using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) - a(i-2) + 2*a(i-3) - a(i-4) */
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        cache[i] = (seq_t)2 * t1 - t2 + (seq_t)2 * t3 - t4;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
