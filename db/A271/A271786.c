/*
 * A271786  Expansion of 2*(1-x)*(2*x^2+4*x+1) / (1-x-x^2)^2.
 * https://oeis.org/A271786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) with a(0)=2,
 *            a(1)=10, a(2)=18, a(3)=38 This recurrence follows from the
 *            generating function 2*(1-x)*(2*x^2+4*x+1)/(1-x-x^2)^2
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise the cache with at least four terms */
    if (cache == NULL) {
        idx_t init = n + 1;
        if (init < 4) init = 4;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure */

        cache[0] = 2;
        cache[1] = 10;
        cache[2] = 18;
        cache[3] = 38;
        cache_size = init;

        for (idx_t i = 4; i < init; ++i) {
            seq_t t1 = (seq_t)2 * cache[i-1] + cache[i-2];
            seq_t t2 = (seq_t)2 * cache[i-3] + cache[i-4];
            cache[i] = t1 - t2;
        }
    }

    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t t1 = (seq_t)2 * cache[i-1] + cache[i-2];
            seq_t t2 = (seq_t)2 * cache[i-3] + cache[i-4];
            cache[i] = t1 - t2;
        }
        cache_size = new_size;
    }

    return cache[n];
}
