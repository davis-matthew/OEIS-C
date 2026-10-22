/*
 * A316688  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) for n>6 Initial terms: a(1)=4,
 *            a(2)=3, a(3)=8, a(4)=9, a(5)=21, a(6)=42
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;           /* undefined below the offset */

    /* Lazy initialization of cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (cache == NULL) {
        cache_size = 7;               /* indices 0..6, index 0 unused */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;            /* allocation failure – unlikely */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;            /* a(1) */
        cache[2] = (seq_t)3;            /* a(2) */
        cache[3] = (seq_t)8;            /* a(3) */
        cache[4] = (seq_t)9;            /* a(4) */
        cache[5] = (seq_t)21;           /* a(5) */
        cache[6] = (seq_t)42;           /* a(6) */
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;            /* allocation failure */
        cache = tmp;

        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)2 * cache[i - 3];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
