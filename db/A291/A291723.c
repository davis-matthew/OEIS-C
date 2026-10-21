/*
 * A291723  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = 1 - S^3.
 * https://oeis.org/A291723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + 3*a(n-5) + 3*a(n-7) + a(n-9) for n >= 10 Initial
 *            values: a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=3, a(5)=1, a(6)=3,
 *            a(7)=6, a(8)=2, a(9)=15
 * Range:     u8 n <= 18, u16 n <= 32, u32 n <= 61, u64 n <= 119,
 *            u128 n <= 235, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[10] = {0, 0, 1, 0, 3, 1, 3, 6, 2, 15};

/* Linear‑space cache for the values of the sequence.  The cache is
   dynamically enlarged the first time a request beyond the current
   capacity is made.  */
static seq_t *cache      = NULL; /* pointer to the cached array     */
static size_t cache_size = 0;     /* number of elements already
                                      computed and stored           */
static size_t cache_cap  = 0;     /* allocated capacity of 'cache'  */

/* Compute a(idx_t n) using the recurrence; the function is
   thread‑safe only in the sense that it uses static data but
   without external synchronization.  */
seq_t a(idx_t n)
{
    /* Initialise the static cache once */
    if (cache == NULL) {
        /* Start with capacity equal to the number of initial values. */
        cache_cap  = 10;
        cache_size = 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* The allocation must succeed for the tests; otherwise abort. */
        if (cache == NULL)
            abort();
        for (size_t i = 0; i < 10; ++i)
            cache[i] = init_vals[i];
    }

    /* If n is already in the cache, return it directly. */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Grow the cache to accommodate index n if necessary. */
    if ((size_t)n >= cache_cap) {
        /* Double the capacity until it is large enough. */
        while ((size_t)n >= cache_cap)
            cache_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (new_cache == NULL)
            abort();                          /* out of memory */
        cache = new_cache;
    }

    /* Compute missing entries up to n using the recurrence. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* For i < 10 the code below would access negative indexes;
           however, that case was handled above by returning early.     */
        seq_t v = cache[i - 3]
                + (seq_t)3 * cache[i - 5]
                + (seq_t)3 * cache[i - 7]
                + cache[i - 9];
        cache[i] = v;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
