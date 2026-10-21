/*
 * A276477  a(n) = a(n-2) + a(n-3) for n >= 3, with a(0) = a(1) = 2, a(2) = 1.
 * https://oeis.org/A276477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 38, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + a(n-3) for n ≥ 3,
   with a(0) = 2, a(1) = 2, a(2) = 1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values.
       ;  cache[i] holds a(i) for i ≤ last. */
    static seq_t *cache = NULL;          /* array of values          */
    static size_t cap   = 0;             /* allocated capacity       */
    static idx_t last   = 0;             /* highest index computed   */

    /* First call: initialise the cache with the three base cases. */
    if (cache == NULL) {
        cap   = 3;                      /* need space for indices 0‑2 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed      */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        last    = 2;                    /* highest computed index   */
    }

    /* If the requested index is already in the cache, we can return it. */
    if (n <= last) return cache[n];

    /* Need to grow the cache so that it can hold index n. */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap;
        if (newcap == 0) newcap = 4;
        while ((size_t)n + 1 > newcap) newcap <<= 1; /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed   */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute any missing terms.  We start from last+1 up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i - 2] + cache[i - 3];
    }
    last = n;   /* update highest computed index */

    return cache[n];
}
