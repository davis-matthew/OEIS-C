/*
 * A297909  Number of n X 2 0..1 arrays with every element equal to 0, 2, 3 or
 *          4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) + 8*a(n-3) + 4*a(n-4) with a(1) = 1, a(2) = 5,
 *            a(3) = 12, a(4) = 37
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of sequence A297909.
   Calls can come in any order; the function memoises
   previously computed values. */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;   /* cache[1]..cache[cache_sz] contain known terms */
    static idx_t cache_sz = 0;    /* highest n that has been computed so far */

    /* initialise cache with the first four terms */
    if (cache == NULL)
    {
        cache_sz = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_sz + 1));
        cache[0] = 0;                 /* unused, keep zero for safety */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)37;
    }

    /* handle terms that are already known */
    if (n <= cache_sz)
        return cache[n];

    /* extend the cache if necessary */
    if (n > cache_sz)
    {
        /* grow the cache to hold values up to n */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (new_cache == NULL)
        {
            /* allocation failed; in a full implementation you might
               handle the error, but for the purposes of this function
               we simply return zero. */
            return (seq_t)0;
        }
        cache = new_cache;

        /* compute new terms using the recurrence */
        for (idx_t i = cache_sz + 1; i <= n; ++i)
        {
            cache[i] = ((seq_t)5) * cache[i - 2]
                     + ((seq_t)8) * cache[i - 3]
                     + ((seq_t)4) * cache[i - 4];
        }

        cache_sz = n;
    }

    return cache[n];
}
