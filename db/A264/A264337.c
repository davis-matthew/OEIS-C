/*
 * A264337  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having index change +-(.,.) 0,0 0,1 or 1,2.
 * https://oeis.org/A264337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) + 16*a(n-2) for n ≥ 3 base values: a(1) = 181,
 *            a(2) = 3567
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External function required by OEIS sequence implementation */
seq_t a(idx_t n)
{
    /* static cache: cache[1] = a(1), cache[2] = a(2), cache[3] = a(3), ... */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* maximum index currently stored */

    /* initialise cache with the two known terms */
    if (cache == NULL)
    {
        const size_t init_size = 3;          /* indices 0..2 */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return 0;                /* out of memory */
        cache[0] = 0;                       /* unused, simplifies recurrence */
        cache[1] = 181;
        cache[2] = 3567;
        cache_size = 2;                     /* largest index stored so far */
    }

    /* if the requested index is already cached, return the value */
    if (n <= cache_size)
        return cache[n];

    /* enlarge cache to hold up to index n */
    const size_t new_size = (size_t)n + 1;  /* need space for 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;                     /* out of memory */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        cache[i] = (seq_t)19 * cache[i - 1] + (seq_t)16 * cache[i - 2];
    }

    cache_size = n;
    return cache[n];
}
