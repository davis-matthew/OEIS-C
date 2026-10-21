/*
 * A254964  Indices of heptagonal numbers (A000566) that are also centered
 *          hexagonal numbers (A003215).
 * https://oeis.org/A254964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A254964
 *  a(n) = a(n-1) + 22*a(n-2) - 22*a(n-3) - a(n-4) + a(n-5)
 *  a(1)=1, a(2)=2, a(3)=14, a(4)=37, a(5)=301
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: store terms 1..cache_len
     * cache[i] corresponds to a(i+1) */
    static seq_t *cache = NULL;      /* pointer to cached values   */
    static idx_t  cache_len = 0;     /* highest computed index     */
    static size_t cache_cap = 0;     /* allocation capacity (in elems) */

    /* ensure capacity for at least n elements */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n) {
            if (new_cap > SIZE_MAX / 2) {   /* avoid overflow */
                new_cap = SIZE_MAX;
                break;
            }
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first five terms if needed */
    if (cache_len < 5) {
        if (cache_len < 1) cache[0] = (seq_t)1;
        if (cache_len < 2) cache[1] = (seq_t)2;
        if (cache_len < 3) cache[2] = (seq_t)14;
        if (cache_len < 4) cache[3] = (seq_t)37;
        if (cache_len < 5) cache[4] = (seq_t)301;
        cache_len = 5;
    }

    /* compute terms up to n if required */
    if (n > cache_len) {
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            /* recurrence uses 1‑based indices:
             * a(k) = a(k-1) + 22*a(k-2) - 22*a(k-3) - a(k-4) + a(k-5)   */
            cache[k-1] =
                cache[k-2]                              /* a(k-1) */
              + (seq_t)22 * cache[k-3]                  /* 22*a(k-2) */
              - (seq_t)22 * cache[k-4]                  /* 22*a(k-3) */
              - cache[k-5]                              /* a(k-4)  */
              + cache[k-6];                             /* a(k-5)  */
        }
        cache_len = n;
    }

    return cache[n-1];
}
