/*
 * A259518  Number of (n+1) X (2+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0001 0011 0101 or 1111.
 * https://oeis.org/A259518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 7*a(n-2) - 10*a(n-3) + 10*a(n-4) + 2*a(n-5)
   initial values: a(1)=38, a(2)=208, a(3)=1122, a(4)=6100, a(5)=33162 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[0] unused, cache[1..k] filled with a(1)..a(k) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* largest index stored (>=0) */

    if (n == 0)
        return 0; /* out of range – behaviour irrelevant */

    /* initialise first 5 terms on the first call */
    if (cache_size == 0) {
        size_t need = 6; /* indices 0..5 */
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache) return 0;
        cache[0]   = 0;          /* dummy */
        cache[1]   = 38;
        cache[2]   = 208;
        cache[3]   = 1122;
        cache[4]   = 6100;
        cache[5]   = 33162;
        cache_size = 5;
    }

    /* if the requested term is already cached, return it */
    if (n <= cache_size)
        return cache[(size_t)n];

    /* grow cache to hold at least up to n */
    size_t new_cap = cache_size;
    while (new_cap < (size_t)n)
        new_cap = (new_cap == 0 ? 5 : new_cap * 2);  /* at least 5 */

    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        cache = NULL;
        cache_size = 0;
        return 0;    /* allocation failed */
    }
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i-1] * (seq_t)7;
        t1       = t1 + cache[i-4] * (seq_t)10;
        t1       = t1 + cache[i-5] * (seq_t)2;
        seq_t t2 = cache[i-2] * (seq_t)7;
        t2       = t2 + cache[i-3] * (seq_t)10;
        cache[i] = t1 - t2;
    }

    cache_size = (idx_t)n;
    return cache[(size_t)n];
}
