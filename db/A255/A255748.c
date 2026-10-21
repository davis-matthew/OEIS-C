/*
 * A255748  Total number of ON states after n generations of cellular
 *          automaton based on triangles in a 60-degree wedge (see Comments
 *          lines for definition).
 * https://oeis.org/A255748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=0; a(2*n)=2*(a(n)+a(n-1)+1)-n; a(2*n+1)=4*a(n)+1-n
 * Range:     u8 n <= 33, u16 n <= 555, u32 n <= 9851, u64 n <= 9851,
 *            u128 n <= 9851, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of known a(n) values      */
static idx_t cache_cap = 0;      /* current capacity of cache       */
static idx_t cache_needed = 0;   /* largest n requested so far      */

/* Grow the cache so that index n is safe to access.  */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;

    /* Determine new capacity (double until it fits). */
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *new_cache;
#if defined(__cplusplus)
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache) abort();   /* allocation failed */

    /* Initialize new slots to zero (sentinel for “not yet computed”). */
    for (idx_t i = 0; i < new_cap; ++i)
        new_cache[i] = 0;

    /* Copy previous values. */
    for (idx_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

#if defined(__cplusplus)
    delete[] cache;
#else
    free(cache);
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

/* Recursive computation with memoisation.  All values are <2^128 (or
 * correspondingly larger if seq_t is an arbitrary-precision integer).    */
static seq_t rec_a(idx_t n)
{
    if (n == 0) return 0;          /* a(0)=0 is known */

    ensure_cache(n);

    if (cache[n] != 0)             /* already computed (sentinel 0 is unused for n>0) */
        return cache[n];

    seq_t val;
    if ((n & 1) == 0) {            /* even : n = 2*k */
        idx_t k = n >> 1;
        seq_t ak  = rec_a(k);
        seq_t ak1 = rec_a(k - 1);
        val = 2 * (ak + ak1 + (seq_t)1) - (seq_t)k;
    } else {                       /* odd  : n = 2*k + 1 */
        idx_t k = n >> 1;          /* (n-1)/2  */
        seq_t ak = rec_a(k);
        val = 4 * ak + (seq_t)1 - (seq_t)k;
    }

    cache[n] = val;                 /* memoise */
    return val;
}

/* Public API required by the build system.  */
seq_t a(idx_t n)
{
    return rec_a(n);
}
