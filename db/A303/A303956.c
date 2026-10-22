/*
 * A303956  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + 2*a(n-4) - 2*a(n-5) -
 *            6*a(n-6) - 4*a(n-7) for n > 8 Initial values: a(1) = 4, a(2) =
 *            5, a(3) = 11, a(4) = 22, a(5) = 49, a(6) = 119, a(7) = 287, a(8)
 *            = 664
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache enough for any number of calls.  The array is grown
       lazily and stored on the heap, thus the size of seq_t does not
       need to be a compile‑time constant. */
    static seq_t *cache   = NULL;   /* pointer to cached values */
    static idx_t  cached  = 0;      /* highest index computed so far */
    static size_t sz      = 0;      /* current allocated size       */

    /* The sequence is defined for n >= 1.  For n == 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Initialise cache on first invocation. */
    if (cache == NULL) {
        /* Allocate space for the initial 8 terms (index 0..8). */
        sz = 9;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;              /* allocation failed – give up */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)49;
        cache[6] = (seq_t)119;
        cache[7] = (seq_t)287;
        cache[8] = (seq_t)664;
        cached = 8;
    }

    /* If the requested term is already in the cache, return it. */
    if (n <= cached)
        return cache[n];

    /* Make sure the cache is large enough for the requested index. */
    if ((size_t)n >= sz) {
        size_t new_sz = sz;
        while ((size_t)n >= new_sz)
            new_sz *= 2;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;      /* allocation failed – give up */
        cache = tmp;
        sz    = new_sz;
    }

    /* Compute terms from cached+1 up to n using the recurrence. */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* The recurrence is valid for i >= 9; for i <= 8 the
           corresponding values are already initialised. */
        seq_t t1 = cache[i - 1];
        seq_t t2 = 2 * cache[i - 2];
        seq_t t3 = 3 * cache[i - 3];
        seq_t t4 = 2 * cache[i - 4];
        seq_t t5 = 2 * cache[i - 5];
        seq_t t6 = 6 * cache[i - 6];
        seq_t t7 = 4 * cache[i - 7];
        cache[i] = t1 + t2 + t3 + t4 - t5 - t6 - t7;
    }
    cached = n;
    return cache[n];
}
