/*
 * A335393  a(1) = 1 and for any n >= 1, a(2*n) = a(n) + 5, a(2*n+1) = a(n) *
 *          3.
 * https://oeis.org/A335393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2*n) = a(n) + 5; a(2*n+1) = a(n) * 3
 * Range:     u8 n <= 38, u16 n <= 1278, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_upto = 0;

    if (n == 0)        /* offset starts at 1; 0 is undefined */
        return (seq_t)0;

    /* first allocation: ensure cache[1] exists */
    if (cache == NULL) {
        /* allocate enough room for indices 0..n (index 0 unused) */
        cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)1;
        cached_upto = 1;
    }

    /* extend cache if needed */
    if (n > cached_upto) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* compute new values */
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            if ((i & 1) == 0)          /* even */
                cache[i] = cache[i / 2] + (seq_t)5;
            else                       /* odd */
                cache[i] = cache[i / 2] * (seq_t)3;
        }
        cached_upto = n;
    }

    return cache[n];
}
