/*
 * A259244  Number of (n+1) X (2+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0011 or 0111.
 * https://oeis.org/A259244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) – 11*a(n-3) – 3*a(n-4) a(1)=21,
 *            a(2)=65, a(3)=192, a(4)=581
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined below the offset */
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  size    = 0;   /* number of entries stored, 1‑based */

    /* initialise the first four terms once */
    if (size == 0) {
        idx_t init = 4;        /* at least the first four terms */
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */

        cache[0] = (seq_t)21;   /* a(1) */
        cache[1] = (seq_t)65;   /* a(2) */
        cache[2] = (seq_t)192;  /* a(3) */
        cache[3] = (seq_t)581;  /* a(4) */
        size = init;
    }

    /* grow the cache if needed */
    if (n > size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */

        cache = tmp;
        /* compute the new terms using the recurrence */
        for (idx_t i = size; i < new_size; ++i) {
            cache[i] =
                (seq_t)3 * cache[i-1]         /* 3*a(i)   */
              + (seq_t)4 * cache[i-2]         /* 4*a(i-1) */
              - (seq_t)11 * cache[i-3]        /* 11*a(i-2)*/
              - (seq_t)3 * cache[i-4];        /* 3*a(i-3) */
        }
        size = new_size;
    }

    return cache[n-1];   /* cache[0] is a(1) */
}
