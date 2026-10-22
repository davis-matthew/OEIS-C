/*
 * A301300  Partial sums of A301299.
 * https://oeis.org/A301300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-5) - 2*a(n-6) - a(n-10) + a(n-11) for n >
 *            10 Initial values: a(0)=1, a(1)=5, a(2)=13, a(3)=26, a(4)=44,
 *            a(5)=66, a(6)=92, a(7)=121, a(8)=155, a(9)=195, a(10)=239
 * Range:     u8 n <= 10, u16 n <= 173, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t  cache_size = 0;
    /* initial terms a(0) to a(10) */
    static const seq_t init[11] = {
        (seq_t)1,
        (seq_t)5,
        (seq_t)13,
        (seq_t)26,
        (seq_t)44,
        (seq_t)66,
        (seq_t)92,
        (seq_t)121,
        (seq_t)155,
        (seq_t)195,
        (seq_t)239
    };

    /* if cache not yet allocated, initialise it */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 11);
        if (!cache) return (seq_t)0;   /* out of memory – unlikely in tests */
        for (size_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        cache_size = 11;
    }

    /* If requested index is already inside cache, just return it */
    if ((size_t)n + 1 <= cache_size)
        return cache[(size_t)n];

    /* Need to grow cache to size n+1 */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!tmp) return (seq_t)0;          /* out of memory – unlikely */
    cache = tmp;

    /* Compute new entries by the recurrence */
    for (size_t i = cache_size; i < new_size; ++i) {
        /* safe because i >= 11 after initialization */
        seq_t val = cache[i-1];
        val += 2 * cache[i-5];
        val -= 2 * cache[i-6];
        val -= cache[i-10];
        val += cache[i-11];
        cache[i] = val;
    }
    cache_size = new_size;

    return cache[(size_t)n];
}
