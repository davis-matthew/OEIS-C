/*
 * A280436  Number of nX4 0..1 arrays with no element equal to more than one
 *          of its horizontal and vertical neighbors, with the exception of
 *          exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 39, u64 n <= 84, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Formula: a(n)=4*a(n-1)-4*a(n-2)-2*a(n-3)+4*a(n-4)-a(n-6)  (n>8)
 *  Initial values: a(1)=2, a(2)=20, a(3)=42, a(4)=80,
 *                  a(5)=138, a(6)=232, a(7)=386, a(8)=640
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache     = NULL;      /* array of cached terms */
static size_t cache_cap = 0;         /* allocated size of cache */
static size_t last_idx  = 0;         /* index of the largest computed term */

/* Allocate or grow the cache to at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap]();
    if (cache) {
        for (size_t i = 0; i <= last_idx; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
    }
#else
    seq_t *new_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
    if (cache) {
        for (size_t i = 0; i <= last_idx; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
#endif

    cache   = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with known initial terms */
static void initialise(void)
{
    if (last_idx > 0)
        return;   /* already initialised */

    ensure_capacity(10);   /* we need indices 0..8 at least */

    /* a(1) .. a(8) as per the sequence definition */
    cache[1] = (seq_t)2u;
    cache[2] = (seq_t)20u;
    cache[3] = (seq_t)42u;
    cache[4] = (seq_t)80u;
    cache[5] = (seq_t)138u;
    cache[6] = (seq_t)232u;
    cache[7] = (seq_t)386u;
    cache[8] = (seq_t)640u;

    last_idx = 8;
}

/* Main entry: compute the n-th term (n>=1) */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n=0, but return 0 for safety */

    size_t idx = (size_t)n;

    initialise();               /* ensure cache has the initial values */

    if (idx <= last_idx)
        return cache[idx];

    /* Grow the cache if required */
    ensure_capacity(idx + 1);

    /* Compute terms sequentially up to idx */
    for (size_t k = last_idx + 1; k <= idx; ++k) {
        /* Use the recurrence: a(k) = 4*a(k-1) -4*a(k-2) -2*a(k-3) +4*a(k-4) - a(k-6) */
        seq_t A = (seq_t)4 * cache[k-1] + (seq_t)4 * cache[k-4];
        seq_t B = (seq_t)4 * cache[k-2] + (seq_t)2 * cache[k-3] + cache[k-6];
        cache[k] = A - B;   /* A >= B ensures no unsigned underflow */
    }

    last_idx = idx;
    return cache[idx];
}
