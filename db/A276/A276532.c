/*
 * A276532  a(n) = (a(n-1) * a(n-6) + a(n-2) * a(n-3) * a(n-4) * a(n-5)) /
 *          a(n-7), with a(0) = a(1) = a(2) = a(3) = a(4) = a(5) = a(6) = 1.
 * https://oeis.org/A276532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1) * a(n-6) + a(n-2) * a(n-3) * a(n-4) * a(n-5)) /
 *            a(n-7) with a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=1
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 15, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n-th term of OEIS A276532 for any n ≥ 0.
   It uses a static cache so that calls can be made in any order
   and the result is computed in O(n) time and memory. */

#include <stdlib.h>

/* seq_t and idx_t are defined in the build environment. */

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Return trivial early values for the base cases. */
    if (n <= 6) return 1;

    /* Static state: cache array, its capacity, and the highest
       index that has already been computed. */
    static seq_t *cache     = NULL;
    static size_t  cap      = 0;   /* number of cached entries */
    static idx_t   last = 6;       /* highest index computed so far */

    /* Ensure the cache is large enough. On the first call we allocate
       room for at least 7 entries (the initial conditions).  On later
       calls we grow the cache if needed. */
    if (cache == NULL || cap < (size_t)(n + 1))
    {
        size_t new_cap = (n + 1 > 7) ? n + 1 : 7;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)   /* allocation failed – return 0 */
            return 0;
        cache = tmp;
        cap   = new_cap;

        /* initialise the known first seven elements */
        cache[0] = cache[1] = cache[2] = cache[3] = cache[4] = cache[5] = cache[6] = (seq_t)1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* a(i) = (a(i-1) * a(i-6) + a(i-2)*a(i-3)*a(i-4)*a(i-5)) / a(i-7) */
        seq_t left  = cache[i-1] * cache[i-6];
        seq_t right = cache[i-2] * cache[i-3] * cache[i-4] * cache[i-5];
        cache[i] = (left + right) / cache[i-7];
    }
    last = n;

    return cache[n];
}
