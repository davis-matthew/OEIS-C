/*
 * A387876  a(n) = Sum_{k=0..n} binomial(4*n+1,6*k).
 * https://oeis.org/A387876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1)+69*a(n-2)+1366*a(n-3)+1359*a(n-4)+1296*a(n-5)
 *            for n>4, with a(0)=1, a(1)=1, a(2)=85, a(3)=1730, a(4)=18565
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* cached values of the sequence */
static size_t cache_size = 0;        /* highest index computed (1 more than max n) */
static size_t cache_cap  = 0;        /* allocated capacity of cache */

/* Ensure the cache has space for indices up to n (inclusive) and compute all
   missing values using the recurrence. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)(n + 1);    /* number of entries required */
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
    /* Compute missing values up to 'need-1' */
    for (size_t i = cache_size; i < need; ++i) {
        if (i == 0)             cache[i] = (seq_t)1;
        else if (i == 1)        cache[i] = (seq_t)1;
        else if (i == 2)        cache[i] = (seq_t)85;
        else if (i == 3)        cache[i] = (seq_t)1730;
        else if (i == 4)        cache[i] = (seq_t)18565;
        else {
            /* recurrence for i >= 5, all required previous values are already computed */
            seq_t v = ((seq_t)6   * cache[i-1]
                    + (seq_t)69  * cache[i-2]
                    + (seq_t)1366 * cache[i-3]
                    + (seq_t)1359 * cache[i-4]
                    + (seq_t)1296 * cache[i-5]);
            cache[i] = v;
        }
    }
    if (cache_size < need) cache_size = need;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
