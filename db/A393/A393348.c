/*
 * A393348  Number of edge cuts in the n-necklace graph.
 * https://oeis.org/A393348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64^n - (24*n + 14)*14^(n-1) Recurrence (order 3):
 *            a(1)=26, a(2)=3228, a(3)=245288 a(n) = 92*a(n-1) - 1988*a(n-2) +
 *            12544*a(n-3) (n ≥ 4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (outside the offset). */
    if (n == 0)
        return 0;

    /* Static cache: number of stored terms (starting from a(1)). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* count of terms stored */

    /* First call: initialise cache with first three terms. */
    if (cache == NULL) {
        size_t init = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * init);
        if (!cache) return 0;            /* allocation failure */
        cache[0] = (seq_t)26;            /* a(1) */
        cache[1] = (seq_t)3228;          /* a(2) */
        cache[2] = (seq_t)245288;        /* a(3) */
        cache_size = init;
    }

    /* If the requested term is already cached, return it. */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Grow cache to accommodate n terms. */
    size_t new_size = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)        /* allocation failure – return 0 as a fallback */
        return 0;
    cache = new_cache;

    /* Compute any missing terms using the recurrence. */
    for (size_t i = cache_size; i < new_size; ++i) {  /* i: zero‑based index */
        idx_t idx = (idx_t)(i + 1);   /* term number (starts at 1) */

        if (idx == 1)
            cache[i] = (seq_t)26;
        else if (idx == 2)
            cache[i] = (seq_t)3228;
        else if (idx == 3)
            cache[i] = (seq_t)245288;
        else
        {
            /* a(n) = 92*a(n-1) - 1988*a(n-2) + 12544*a(n-3) */
            /* Compute as (92*a(n-1) + 12544*a(n-3)) - 1988*a(n-2) */
            seq_t t1 = (seq_t)92   * cache[i - 1] + (seq_t)12544 * cache[i - 3];
            seq_t t2 = (seq_t)1988 * cache[i - 2];
            cache[i] = t1 - t2;
        }
    }

    cache_size = new_size;
    return cache[n - 1];
}
