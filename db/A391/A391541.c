/*
 * A391541  a(n) = Sum_{k=0..floor(2*n/5)} binomial(2*n-3*k,2*k).
 * https://oeis.org/A391541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=1, a(3)=4, a(4)=11
   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) + a(n-5)  for n ≥ 5   */
#include <stdlib.h>

/* Static cache for computed terms */
static seq_t *cache = NULL;      /* array of values          */
static size_t capacity = 0;      /* allocated array size    */
static size_t computed = 0;      /* highest index computed  */

/* Initial known values */
static const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)11 };

seq_t a(idx_t n)
{
    /* Direct returns for indices below 5 */
    if (n < 5)
        return init[n];

    /* Ensure the cache is allocated and has enough space */
    if (capacity == 0)
    {
        capacity = (size_t)n + 1;
        if (capacity < 5) capacity = 5;

        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* sanity guard */

        /* copy the first five known terms */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init[i];
        computed = 4;                       /* largest index computed so far */
    }
    else if ((size_t)n + 1 > capacity)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* sanity guard */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i)
    {
        seq_t val = (seq_t)2 * cache[i-1]
                    - cache[i-2]
                    + (seq_t)4 * cache[i-3]
                    + cache[i-5];
        cache[i] = val;
    }
    computed = (size_t)n;

    return cache[n];
}
