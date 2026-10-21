/*
 * A262242  Triangular numbers representable as 2^x + 2^y.
 * https://oeis.org/A262242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A262242: Triangular numbers representable as 2^x + 2^y.  
 * Recurrence:
 *   a(1)=3, a(2)=6, a(3)=10, a(4)=36, a(5)=66, a(6)=136, a(7)=528,
 *   a(n)=6*a(n-1)-8*a(n-2)   for n>7.
 * The function returns a(n) for any n≥1; values are cached for reuse. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* pointer to cached terms (a(1) in cache[0]) */
static idx_t cache_len = 0;      /* number of terms currently cached */
static idx_t cache_cap = 0;      /* allocated capacity */

/* Helper to grow the cache to at least new_cap terms */
static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    /* Allocate or reallocate memory */
    seq_t *new_cache = NULL;
    if (cache == NULL)
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    else
        new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));

    /* In case realloc fails, we keep the old cache (caller will handle) */
    if (new_cache == NULL)
        return;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize the first seven terms on first use */
static void init_cache(void)
{
    grow_cache(8); /* capacity at least 8 for convenience */

    cache[0] = (seq_t)3;        /* a(1) */
    cache[1] = (seq_t)6;        /* a(2) */
    cache[2] = (seq_t)10;       /* a(3) */
    cache[3] = (seq_t)36;       /* a(4) */
    cache[4] = (seq_t)66;       /* a(5) */
    cache[5] = (seq_t)136;      /* a(6) */
    cache[6] = (seq_t)528;      /* a(7) */
    cache_len = 7;
}

seq_t a(idx_t n)
{
    /* Ensure cache is initialized */
    if (cache == NULL)
        init_cache();

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1U];

    /* Compute terms up to n */
    /* Ensure enough capacity */
    if (n > cache_cap)
        grow_cache(n + 1); /* allocate one extra for 0-based index */

    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* a(i) = 6*a(i-1) - 8*a(i-2)   for i > 7 */
        seq_t val = (seq_t)6 * cache[i - 2] - (seq_t)8 * cache[i - 3];
        cache[i - 1] = val;
    }

    /* Update cached length */
    cache_len = n;

    return cache[n - 1U];
}
