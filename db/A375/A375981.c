/*
 * A375981  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 1, 4, or 5.
 * https://oeis.org/A375981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-6) + a(n-8) - a(n-11) (for
 *            n >= 11, with a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=8, a(5)=11,
 *            a(6)=14, a(7)=19, a(8)=25, a(9)=34, a(10)=49)
 * Range:     u8 n <= 14, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;          /* cache of previously computed terms   */
static size_t cache_cap = 0;         /* allocated size of cache              */
static size_t cache_len = 0;         /* number of valid terms stored in cache */

/* Ensure the cache has space for indices 0 .. n-1.  Uses realloc for
   simplicity; called only a few times for large n. */
static void __ensure_cache(idx_t n)
{
    size_t need = (size_t)n;
    if (need <= cache_cap)
        return;
    /* Double the capacity for amortized O(1) allocation cost. */
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 11 terms (0..10).  This function is
   idempotent; it will do nothing on subsequent calls. */
static void __init_cache(void)
{
    if (cache_len > 0)
        return;
    __ensure_cache(11 + 1);           /* need indices 0..10 (11 terms)       */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)5;
    cache[4] = (seq_t)8;
    cache[5] = (seq_t)11;
    cache[6] = (seq_t)14;
    cache[7] = (seq_t)19;
    cache[8] = (seq_t)25;
    cache[9] = (seq_t)34;
    cache[10] = (seq_t)49;
    cache_len = 11;
}

seq_t a(idx_t n)
{
    __init_cache();
    if ((idx_t)n < (idx_t)cache_len)
        return cache[(size_t)n];

    /* Ensure the cache can hold all terms up to n. */
    __ensure_cache((size_t)n + 1);

    /* Compute missing terms up to n inclusive. */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        /* The recurrence is defined only for i >= 11.  For i < 11 the
           values are already in the cache (handled by the caller).       */
        seq_t val = cache[i - 1] +      /* a(i-1) */
                    cache[i - 3] -      /* a(i-3) */
                    cache[i - 4] +      /* -a(i-4) */
                    cache[i - 6] +      /* a(i-6) */
                    cache[i - 8] -      /* a(i-8) */
                    cache[i - 11];      /* -a(i-11) */
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
