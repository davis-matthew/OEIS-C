/*
 * A377195  Expansion of 1/(1 - 4*x*(1+x)^2)^(5/2).
 * https://oeis.org/A377195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*(2*n+3)*a(n-1) + 8*(n+3)*a(n-2) + 2*(2*n+9)*a(n-3))/n
 *            with initial values a(0)=1, a(1)=10, a(2)=90
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached values of the sequence */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;     /* current capacity of cache (size in elements) */
static idx_t cache_last = 0;    /* last index for which cache[i] is valid */
static int initialized = 0;    /* 0 = not yet initialised, 1 = initialised */

/* ensure that the cache can hold indices up to 'need' (inclusive) */
static void ensure_capacity(idx_t need)
{
    /* capacity and needed indices are of type idx_t (uint64_t). */
    if (need + 1 <= cache_cap)
        return;

    idx_t new_cap = need + 1;
    /* reuse realloc for both C and C++ builds – cast result when needed */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* The recurrence uses indices up to n-3, so we need cache entries
       for 0,1,2 at all times.  Allocate the minimum space first. */
    if (cache_cap < 3)
        ensure_capacity(2);

    /* initialise base terms once */
    if (!initialized) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)90;
        cache_last = 2;
        initialized = 1;
    }

    /* already computed? */
    if (n <= cache_last)
        return cache[n];

    /* ensure cache is large enough for the requested index */
    ensure_capacity(n);

    /* compute remaining values up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];

#if defined(__cplusplus)
        /* Compile-time big-integer type: use normal integer arithmetic. */
        cache[i] = ( ( (seq_t)2 * ( (seq_t)(2 * i + 3) ) * a1 )
                   + ( (seq_t)8 * ( (seq_t)(i + 3) ) * a2 )
                   + ( (seq_t)2 * ( (seq_t)(2 * i + 9) ) * a3 )
                  ) / (seq_t)i;
#else   /* C build (typical 64‑bit unsigned integer). Use 128‑bit temp. */
        __int128 num = 0;
        num += (__int128)2 * (2 * i + 3) * (__int128)a1;
        num += (__int128)8 * (i + 3)  * (__int128)a2;
        num += (__int128)2 * (2 * i + 9) * (__int128)a3;
        cache[i] = (seq_t)(num / (__int128)i);
#endif
    }
    cache_last = n;
    return cache[n];
}
