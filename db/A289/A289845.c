/*
 * A289845  p-INVERT of A079977, where p(S) = 1 - S - S^2.
 * https://oeis.org/A289845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) + a(n-4) - a(n-5) - 2*a(n-6) -
 *            a(n-8) for n ≥ 8, with initial values: a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=9, a(4)=19, a(5)=43, a(6)=91, a(7)=202
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)9,
        (seq_t)19, (seq_t)43, (seq_t)91, (seq_t)202
    };

    /* static cache of already-computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 0;

    /* allocate cache if first call */
    if (cache == NULL) {
        cache_size = (n > 7 ? n + 1 : 8);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */

        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        last_computed = 7;

        /* compute up to requested n if needed */
        if (n <= last_computed)
            return cache[n];
        for (idx_t i = 8; i <= n; ++i) {
            seq_t sum_pos = cache[i-1]
                          + (seq_t)3 * cache[i-2]
                          + cache[i-4];
            seq_t sum_neg = cache[i-3]
                          + cache[i-5]
                          + (seq_t)2 * cache[i-6]
                          + cache[i-8];
            cache[i] = sum_pos - sum_neg;
        }
        last_computed = n;
        return cache[n];
    }

    /* cache already exists */
    if (n <= last_computed)
        return cache[n];

    /* grow cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size < n + 1)
            new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();          /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum_pos = cache[i-1]
                      + (seq_t)3 * cache[i-2]
                      + cache[i-4];
        seq_t sum_neg = cache[i-3]
                      + cache[i-5]
                      + (seq_t)2 * cache[i-6]
                      + cache[i-8];
        cache[i] = sum_pos - sum_neg;
    }
    last_computed = n;
    return cache[n];
}
