/*
 * A254965  Indices of centered hexagonal numbers (A003215) that are also
 *          heptagonal numbers (A000566).
 * https://oeis.org/A254965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 22*a(n-2) – 22*a(n-3) – a(n-4) + a(n-5); Initial
 *            values: a(1)=1, a(2)=2, a(3)=13, a(4)=34, a(5)=275
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache holds values a(1)..a(cache_size) in indices 0..cache_size-1 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of cached elements */

    if (n == 0) return 0;          /* undefined for the offset */

    /* remember old size before possible reallocation */
    idx_t old_size = cache_size;

    /* allocate or grow the cache so that it holds at least n elements */
    if (cache == NULL) {
        cache = (seq_t *)malloc(n * sizeof(seq_t));
        if (!cache) abort();
        cache_size = n;
    } else if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = n;
    }

    /* initialise the first five terms if they have not been set yet */
    if (old_size < 5) {
        if (old_size <= 0 && cache_size >= 1) cache[0] = 1;
        if (old_size <= 1 && cache_size >= 2) cache[1] = 2;
        if (old_size <= 2 && cache_size >= 3) cache[2] = 13;
        if (old_size <= 3 && cache_size >= 4) cache[3] = 34;
        if (old_size <= 4 && cache_size >= 5) cache[4] = 275;
    }

    /* compute any missing terms up to a(n) */
    for (idx_t i = 5; i < n; ++i) {
        seq_t val = cache[i - 1];                 /* a(n-1) */
        val += 22 * cache[i - 2];                 /* +22*a(n-2) */
        val -= 22 * cache[i - 3];                 /* -22*a(n-3) */
        val -=  cache[i - 4];                     /* -  a(n-4) */
        val +=  cache[i - 5];                     /* +  a(n-5) */
        cache[i] = val;
    }

    return cache[n - 1];
}
