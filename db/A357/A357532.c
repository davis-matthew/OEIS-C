/*
 * A357532  a(n) = Sum_{k=0..floor(n/3)} (n-2*k)!/(n-3*k)!.
 * https://oeis.org/A357532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 33, u64 n <= 55, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=a(2)=1; for n>2 :
   a(n) = (2*a(n-1) + n*a(n-3) + 1)/3
   The code uses 128‑bit arithmetic when seq_t is a 64‑bit integer
   to avoid overflow; it works unchanged for arbitrary‑precision
   seq_t types. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* dynamic array of cached values   */
static idx_t  cache_size = 0;        /* allocated size of cache          */
static idx_t  computed   = 0;        /* highest index computed so far    */
static int    initialized = 0;       /* flag indicating first call      */

/* Ensure cache has space for at least indices 0..n. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;
    idx_t newsize = cache_size ? cache_size : 4;
    while (newsize <= n)
        newsize <<= 1;
    seq_t *newptr = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!newptr) /* allocation failed – abort */
        abort();
    cache = newptr;
    cache_size = newsize;
}

seq_t
a(idx_t n)
{
    /* First call: initialise cache with base values. */
    if (!initialized) {
        cache_size = 4;                    /* enough for indices 0..3 when needed */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        computed = 2;
        initialized = 1;
    }

    /* If the value is already computed, return it. */
    if (n <= computed)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 2) {                   /* keep base cases safe from overflow */
            cache[i] = (seq_t)1;
        } else {
#ifdef __cplusplus
            /* For C++ builds (possible arbitrary‑precision seq_t) we rely
               on the class's built‑in arithmetic operators. */
            seq_t term1 = seq_t(2) * cache[i-1];
            seq_t term2 = seq_t(i) * cache[i-3];
            cache[i]   = (term1 + term2 + seq_t(1)) / seq_t(3);
#else
            /* For C builds with 64‑bit seq_t we use 128‑bit intermediate. */
            unsigned __int128 t1 = (unsigned __int128)2 * cache[i-1];
            unsigned __int128 t2 = (unsigned __int128)i * cache[i-3];
            unsigned __int128 sum = t1 + t2 + 1;
            cache[i] = (seq_t)(sum / 3);
#endif
        }
    }
    computed = n;
    return cache[n];
}
