/*
 * A259637  Number of (n+2) X (3+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 or 00000001.
 * https://oeis.org/A259637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)+10*a(n-3)+4*a(n-4)-2*a(n-5)
 *                -12*a(n-6)-5*a(n-7)-4*a(n-8)+4*a(n-9)  for n > 10
 * Initial values:
 * a(1)=69,  a(2)=171,  a(3)=581,  a(4)=1483,  a(5)=3849,
 * a(6)=11261, a(7)=30957, a(8)=82977, a(9)=231015, a(10)=639787
 * This implementation memoises all values up to the largest request
 * and works for any of the supported seq_t types.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* return 0 for undefined indices */
    if (n < 1)
        return (seq_t)0;

    /* initial 10 terms */
    static const seq_t init[10] = {
        (seq_t)69,
        (seq_t)171,
        (seq_t)581,
        (seq_t)1483,
        (seq_t)3849,
        (seq_t)11261,
        (seq_t)30957,
        (seq_t)82977,
        (seq_t)231015,
        (seq_t)639787
    };

    /* If the request is within the initial block, return directly */
    if (n <= 10)
        return init[n-1];

    /* Memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of values currently stored (>=10) */

    /* First time we need a value beyond the initial block,
       allocate the cache and initialise the first 10 terms. */
    if (cache == NULL) {
        cache_size = (idx_t)10;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;              /* allocation failed */

        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init[i];
    }

    /* If the cache is too small, grow it and compute the missing terms.
       cache_size holds the previous number of entries (>=10). */
    if (n > cache_size) {
        idx_t old_size = cache_size;
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;              /* realloc failed */

        cache = tmp;
        /* Compute from old_size+1 to n inclusive */
        for (idx_t k = old_size + 1; k <= n; ++k) {
            /* k indexes the desired a(k); array index is k-1          */
            /* cache indices are 0‑based: a(1) -> cache[0]            */
            cache[k-1] =
                cache[k-2]          /* a(k-1) */
              + cache[k-3]          /* a(k-2) */
              + 10 * cache[k-4]     /* a(k-3) */
              + 4  * cache[k-5]     /* a(k-4) */
              - 2  * cache[k-6]     /* a(k-5) */
              - 12 * cache[k-7]     /* a(k-6) */
              - 5  * cache[k-8]     /* a(k-7) */
              - 4  * cache[k-9]     /* a(k-8) */
              + 4  * cache[k-10];   /* a(k-9) */
        }
        cache_size = n;
    }

    return cache[n-1];
}
