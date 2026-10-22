/*
 * A387745  a(n) = (1/6) * Sum_{k=0..n-1} binomial(6*n,6*k+1).
 * https://oeis.org/A387745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (1/6) * Sum_{k=0}^{n-1} binomial(6*n, 6*k+1)
 * Equivalent recurrence: a(n) = 38*a(n-1)+1691*a(n-2)-1728*a(n-3)  (n>3)
 * Initial values: a(0)=0, a(1)=1, a(2)=134, a(3)=6735
 */

#include <stdlib.h>
#include <stdint.h>

/* External integer types supplied by the build */
extern seq_t a(idx_t n);

/* Function implementation */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated array size */
    static size_t term_count = 0; /* number of terms currently stored */

    /* Initialise the cache with the first four values */
    if (!cache)
    {
        capacity   = 4;
        term_count = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)134;
        cache[3] = (seq_t)6735;
    }

    /* Ensure the allocated array is large enough for index n */
    if (n + 1 > capacity)
    {
        size_t new_capacity = capacity * 2;
        while (new_capacity <= n + 1) new_capacity *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) exit(1);
        cache     = tmp;
        capacity  = new_capacity;
    }

    /* Compute missing terms up to n */
    if (n >= term_count)
    {
        for (idx_t i = term_count; i <= n; ++i)
        {
            cache[i] = ( (seq_t)38   * cache[i-1] )
                     + ( (seq_t)1691 * cache[i-2] )
                     - ( (seq_t)1728 * cache[i-3] );
        }
        term_count = n + 1;
    }

    return cache[n];
}
