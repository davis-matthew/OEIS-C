/*
 * A301294  Partial sums of A301293.
 * https://oeis.org/A301294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 3*a(n-4) + a(n-5);
 *            Initial values: a(0)=1, a(1)=5, a(2)=14, a(3)=28, a(4)=46
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of valid terms stored  */
    static size_t cache_cap  = 0;   /* capacity of cache array          */

    /* first call: initialise cache with 5 known terms */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)28;
        cache[4] = (seq_t)46;
        cache_size = 5;
    }

    /* if requested term already cached, return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* ensure the cache has enough space */
    size_t required = (size_t)n + 1;
    if (required > cache_cap)
    {
        size_t new_cap = cache_cap;
        while (new_cap < required) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n inclusive using the recurrence */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        /* recurrence: 3*a(i-1) - 4*a(i-2) + 4*a(i-3) - 3*a(i-4) + a(i-5) */
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t a5 = cache[i-5];

        seq_t pos = 3*a1 + 4*a3 + a5;    /* all positive terms */
        seq_t neg = 4*a2 + 3*a4;          /* all negative terms */
        cache[i] = pos - neg;            /* subtraction safe: pos > neg */
    }
    cache_size = required;

    return cache[(size_t)n];
}
