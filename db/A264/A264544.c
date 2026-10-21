/*
 * A264544  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change -1,1 -1,2 1,0 or 0,-1.
 * https://oeis.org/A264544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-4) (n ≥ 5) with a(1)=1, a(2)=2, a(3)=5,
 *            a(4)=10
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A264544 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* stores a(1), a(2), ...                     */
static size_t cache_len = 0;   /* number of values currently cached          */
static size_t cache_cap = 0;   /* capacity of the allocated array            */

seq_t a(idx_t n)
{
    if (n == 0)            /* below offset – behaviour undefined, return 0 */
        return (seq_t)0;

    size_t target = (size_t)n;   /* number of terms to have ready           */

    /* Grow the cache if necessary */
    if (target > cache_len) {
        /* ensure capacity */
        size_t new_cap = cache_cap > 0 ? cache_cap : 16;
        while (new_cap < target)
            new_cap <<= 1;           /* double until enough */

        /* allocate or reallocate */
        if (cache == NULL)
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

        if (cache == NULL)
            abort();                 /* unrecoverable if memory allocation fails */

        cache_cap = new_cap;

        /* compute the missing values */
        for (size_t i = cache_len; i < target; ++i) {
            if (i == 0)            /* a(1) */
                cache[i] = (seq_t)1;
            else if (i == 1)        /* a(2) */
                cache[i] = (seq_t)2;
            else if (i == 2)        /* a(3) */
                cache[i] = (seq_t)5;
            else if (i == 3)        /* a(4) */
                cache[i] = (seq_t)10;
            else
                cache[i] = cache[i-1] * (seq_t)2 + cache[i-4]; /* recurrence */
        }

        cache_len = target;
    }

    return cache[n-1];
}
