/*
 * A260644  Four steps forward, three steps back.
 * https://oeis.org/A260644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-7) - a(n-8), for n > 7 a(0) = 0, a(1) = 1,
 *            a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 3, a(6) = 2, a(7) = 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of cached elements */

seq_t a(idx_t n)
{
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    size_t new_len = (size_t)(n + 1);
    if (cache_len == 0) {
        cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!cache) { abort(); }
    } else if (new_len > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { abort(); }
        cache = tmp;
    }

    static const seq_t base[8] = {0, 1, 2, 3, 4, 3, 2, 1};
    size_t i;
    for (i = cache_len; i <= (size_t)n; ++i) {
        if (i < 8) {
            cache[i] = base[i];
        } else {
            cache[i] = cache[i - 1] + cache[i - 7] - cache[i - 8];
        }
    }
    cache_len = new_len;
    return cache[(size_t)n];
}
