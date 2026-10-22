/*
 * A386489  Expansion of (1-x)/((1+x+2*x^2)*(1-4*x+x^2)).
 * https://oeis.org/A386489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 7*a(n-3) - 2*a(n-4) with initial
 *            values a(0)=1, a(1)=2, a(2)=7, a(3)=30
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_comp = (idx_t)-1;   /* indicates cache uninitialized */

    if (cache == NULL) {
        /* Allocate space for the first four terms */
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)30;
        max_comp = 3;
    }

    if (n <= max_comp) {
        return cache[n];
    }

    /* Grow the cache to hold up to index n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;       /* allocation failed */
    cache = tmp;

    /* Compute missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        unsigned __int128 val = (unsigned __int128)cache[i-1] * 3
                              + (unsigned __int128)cache[i-2]
                              + (unsigned __int128)cache[i-3] * 7
                              - (unsigned __int128)cache[i-4] * 2;
        cache[i] = (seq_t)val;
    }

    max_comp = n;
    return cache[n];
}
