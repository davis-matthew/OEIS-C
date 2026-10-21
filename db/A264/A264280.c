/*
 * A264280  Number of (n+1) X (3+1) arrays of permutations of 0..n*4+3 with
 *          each element having directed index change 0,0 0,1 1,0 or -1,-2.
 * https://oeis.org/A264280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + 4*a(n-3), with initial values
 *            a(1)=8, a(2)=32, a(3)=132
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For the OEIS offset a(1)=8, a(0) is undefined but we return 0 for safety. */
#include <stdlib.h>

/* External linkage specified by the exercise */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* The sequence is defined for n >= 1 */
    if (n == 0)
        return 0;

    /* First call: allocate cache large enough for the requested n */
    if (cache == NULL) {
        /* Ensure cache at least covers the first three terms */
        cache_size = (n > 3) ? n + 1 : 4;          /* indices 0 .. n */
        cache       = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            exit(1);                               /* allocation failure */

        cache[0] = 0;        /* unused */
        cache[1] = 8;
        cache[2] = 32;
        cache[3] = 132;

        /* If we allocated more than 4 slots, compute all entries up to n */
        if (cache_size > 4) {
            for (idx_t i = 4; i < cache_size; ++i)
                cache[i] = 3 * cache[i - 1] + 4 * cache[i - 2] + 4 * cache[i - 3];
        }
    }
    /* Subsequent calls that request a larger n */
    else if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache)
            exit(1);                               /* allocation failure */

        cache = new_cache;
        /* Compute new entries only */
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = 3 * cache[i - 1] + 4 * cache[i - 2] + 4 * cache[i - 3];
        cache_size = n + 1;
    }

    /* Return the requested value */
    return cache[n];
}
