/*
 * A336102  Number of inseparable multisets of size n covering an initial
 *          interval of positive integers.
 * https://oeis.org/A336102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - 4*a(n-4) for n > 5 Initial values: a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=3, a(5)=3
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 59, u64 n <= 121,
 *            u128 n <= 247, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t last_computed = 0;

    /* initial cache setup */
    if (cache == NULL) {
        cache_len = 6;                       /* indices 0 .. 5 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure (unlikely) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)3;
        last_computed = 5;
    }

    /* if the value has already been computed, return it */
    if ((size_t)n <= last_computed)
        return cache[(size_t)n];

    /* resize cache if necessary */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = cache_len * 2;
        if (new_len < (size_t)(n + 1))
            new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing values up to n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t diff = cache[i - 2] - cache[i - 4];
        cache[i] = diff + diff + diff + diff;  /* 4*diff works for all seq_t types */
    }
    last_computed = (size_t)n;

    return cache[(size_t)n];
}
