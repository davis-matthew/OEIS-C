/*
 * A334157  Row sums of array A158777.
 * https://oeis.org/A334157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A334157: Row sums of array A158777
   Recurrence: a(n+5) - (n+6)a(n+4) + (n+4)a(n+3) - (n+5)(n+2)(n+3)(n+4)a(n+1)
                + (n+1)(n+2)(n+3)(n+4)a(n) = 0
   Initial values: a(0)=1, a(1)=2, a(2)=5, a(3)=16, a(4)=89
   Computes a(n) for any n >= 0, memoizing results for efficiency. */

#include <stdlib.h>

/* Helper macro for safe accumulation of cache size */
#define MAX(a,b) ((a) > (b) ? (a) : (b))

static seq_t *cache = NULL;   /* memoized sequence values */
static idx_t cache_len = 0;   /* number of values currently stored */
static idx_t cache_cap = 0;   /* allocated capacity of cache */

/* Ensure cache has capacity for at least 'needed' elements. */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = MAX(cache_cap * 2, needed);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)         /* out of memory – return zero value */
        return;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize the cache with the first five known terms */
static void init_cache(void)
{
    ensure_cache_capacity(5);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)5;
    cache[3] = (seq_t)16;
    cache[4] = (seq_t)89;
    cache_len = 5;
}

seq_t a(idx_t n)
{
    /* Lazy initialization of the cache */
    if (cache_len == 0)
        init_cache();

    /* If the requested value is already cached, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* Extend cache up to the requested index using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t m = i - 5;           /* current n in the recurrence */

        /* (m+6)*a(m+4) */
        seq_t term1 = cache[m + 4] * (seq_t)(m + 6);

        /* - (m+4)*a(m+3) */
        seq_t term2 = cache[m + 3] * (seq_t)(m + 4);

        /* + (m+5)(m+2)(m+3)(m+4)*a(m+1) */
        seq_t term3 = cache[m + 1] * (seq_t)(m + 5);
        term3 *= (seq_t)(m + 2);
        term3 *= (seq_t)(m + 3);
        term3 *= (seq_t)(m + 4);

        /* - (m+1)(m+2)(m+3)(m+4)*a(m) */
        seq_t term4 = cache[m] * (seq_t)(m + 1);
        term4 *= (seq_t)(m + 2);
        term4 *= (seq_t)(m + 3);
        term4 *= (seq_t)(m + 4);

        /* a(i) = term1 - term2 + term3 - term4 */
        seq_t val = term1 - term2 + term3 - term4;

        /* Store the computed value */
        ensure_cache_capacity(i + 1);
        cache[i] = val;
    }
    cache_len = MAX(cache_len, n + 1);

    return cache[n];
}
