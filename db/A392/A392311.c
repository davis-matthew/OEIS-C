/*
 * A392311  a(n) = Sum_{k=0..floor(3*n/5)} (k+1) * binomial(k,3*n-5*k).
 * https://oeis.org/A392311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(3*n/5)} (k+1)*binomial(k, 3*n-5*k)
 *            Implemented with the linear recurrence of order 12. Initial
 *            values: a(0)=1, a(1)=0, a(2)=2, a(3)=0, a(4)=3, a(5)=4, a(6)=4,
 *            a(7)=20, a(8)=5, a(9)=60, a(10)=13, a(11)=140
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 53, u64 n <= 110,
 *            u128 n <= 225, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t init_vals[12] = {
        1,0,2,0,3,4,4,20,5,60,13,140
    };

    /* initialise cache with the first 12 terms */
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
    }

    /* if the requested n is already computed, return it */
    if (n < cache_size) return cache[n];

    /* ensure the cache is large enough to hold up to n */
    idx_t old_size = cache_size;
    if ((idx_t)(n + 1) > cache_size) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_size = new_sz;
    }

    /* compute new terms using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t pos = 0;
        pos += cache[i-2] * 6;
        pos += cache[i-5] * 2;
        pos += cache[i-6] * 20;
        pos += cache[i-9] * 6;
        pos += cache[i-10] * 5;

        seq_t neg = 0;
        neg += cache[i-4] * 15;
        neg += cache[i-7] * 6;
        neg += cache[i-8] * 15;
        neg += cache[i-11] * 2;
        neg += cache[i-12];

        cache[i] = pos - neg;
    }

    return cache[n];
}
