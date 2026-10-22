/*
 * A304207  a(1)=17; for n>1, a(n) = (a(n-1)^2 - 1)/2 if n is even, a(n-1) + 1
 *          if n is odd.
 * https://oeis.org/A304207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(1) = 17;
 *   for n > 1:
 *       if n is even: a(n) = (a(n-1)^2 - 1) / 2
 *       if n is odd : a(n) = a(n-1) + 1
 *   This implements the OEIS definition of A304207.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] holds a(1) */
    static seq_t *cache     = NULL;   /* array of computed terms */
    static size_t cache_cap = 0;      /* capacity of cache */
    static idx_t  cache_len = 0;      /* number of terms stored */

    if (n == 0) /* undefined, but harmless for non‑positive indices */
        return (seq_t)0;

    /* enlarge the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n > 1 ? (size_t)n * 2 : 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    while (cache_len < n) {
        idx_t i = cache_len;                /* zero‑based index, i+1 == current n */

        if (i == 0) {                       /* a(1) */
            cache[0] = (seq_t)17;
        } else {
            /* current n is i+1 */
            if (((i + 1) & 1) == 0) {       /* even n */
                seq_t prev = cache[i - 1];
                cache[i] = (prev * prev - (seq_t)1) / (seq_t)2;
            } else {                        /* odd n */
                cache[i] = cache[i - 1] + (seq_t)1;
            }
        }
        ++cache_len;
    }

    return cache[n - 1];
}
