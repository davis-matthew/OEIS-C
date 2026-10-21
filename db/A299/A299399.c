/*
 * A299399  a(n) = a(n-1)*a(n-2)*a(n-3)*a(n-4); a(0..3) = (1, 1, 2, 3).
 * https://oeis.org/A299399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*a(n-2)*a(n-3)*a(n-4), a(0) = 1, a(1) = 1, a(2) =
 *            2, a(3) = 3
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS A299399.
   The result is cached so that arbitrary order of calls is handled. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* number of elements currently stored */

    /* the required number of elements (indices 0..n inclusive) */
    size_t needed = (size_t)n + 1;

    /* if cache is too small, grow it */
    if (cache_size < needed)
    {
        size_t old_size = cache_size;

        /* new size (simple, size needed + 1) */
        size_t new_size = needed;

        /* reallocate */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                       /* out‑of‑memory: terminate */

        cache = tmp;
        cache_size = new_size;

        /* initialise base values if they are not yet present */
        if (old_size <= 4)
        {
            /* values for indices 0..3 */
            seq_t init[4] = {(seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3};
            size_t upto = 4;
            if (new_size < upto) upto = new_size;
            for (size_t i = old_size; i < upto; ++i)
                cache[i] = init[i];
        }

        /* compute new terms from the first missing index up to n */
        size_t start = (old_size > 4) ? old_size : 4;
        if (start < old_size) start = old_size;   /* defensive */
        for (size_t i = start; i < new_size; ++i)
        {
            cache[i] = cache[i-1] * cache[i-2] * cache[i-3] * cache[i-4];
        }
    }

    return cache[(size_t)n];
}
