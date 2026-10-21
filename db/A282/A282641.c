/*
 * A282641  Number of nX2 0..1 arrays with no 1 equal to more than one of its
 *          king-move neighbors.
 * https://oeis.org/A282641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 4*a(n-3); Initial values: a(1)=4,
 *            a(2)=11, a(3)=27
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache the sequence values for fast repeated access. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of terms currently stored */
    static size_t capacity = 0;    /* allocated capacity of cache */

    if (n == 0)
        return (seq_t)0;          /* undefined, but avoid UB */

    /* First time: allocate space for the initial terms */
    if (cache == NULL) {
        capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)4;      /* a(1) */
        cache[1] = (seq_t)11;     /* a(2) */
        cache[2] = (seq_t)27;     /* a(3) */
        cached_len = 3;
    }

    /* If we need more terms, grow the cache and compute them */
    if (n - 1 >= cached_len) {
        idx_t required = n;              /* number of terms we need */

        /* Grow the allocated storage as needed */
        if ((idx_t)capacity < required) {
            size_t new_cap = capacity ? capacity : 1;
            while ((idx_t)new_cap < required)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;          /* allocation failure */
            cache = tmp;
            capacity = new_cap;
        }

        /* Compute new terms using the recurrence */
        for (idx_t m = cached_len; m < required; ++m) {
            cache[m] = cache[m-1]
                     + (seq_t)3 * cache[m-2]
                     + (seq_t)4 * cache[m-3];
        }
        cached_len = required;
    }

    return cache[n-1];   /* zero‑based array index */
}
