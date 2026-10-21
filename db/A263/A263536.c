/*
 * A263536  Row sum of an equilateral triangle tiled with the 3,4,5
 *          Pythagorean triple.
 * https://oeis.org/A263536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-4), for n>4 Initial values: a(1)=5,
 *            a(2)=7, a(3)=12, a(4)=17
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == 0) return 0; /* undefined for offset */

    size_t target = (size_t)n;

    if (target > cache_size) {
        size_t new_size = target;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – abort safely */
            abort();
        }
        /* fill new entries */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                new_cache[i] = 5;
            } else if (i == 1) {
                new_cache[i] = 7;
            } else if (i == 2) {
                new_cache[i] = 12;
            } else if (i == 3) {
                new_cache[i] = 17;
            } else {
                new_cache[i] = new_cache[i - 1] + new_cache[i - 3] - new_cache[i - 4];
            }
        }
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[target - 1];
}
