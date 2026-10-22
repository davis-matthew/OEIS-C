/*
 * A375320  Expansion of (1 + x)/(1 - x^3*(1 + x)^4).
 * https://oeis.org/A375320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + 4*a(n-4) + 6*a(n-5) + 4*a(n-6) + a(n-7), a(0) =
 *            1, a(1) = 1, a(2) = 0, a(3) = 1, a(4) = 5, a(5) = 10, a(6) = 11
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

static void init_base(void)
{
    const size_t base_len = 7;
    cache = (seq_t *)malloc(base_len * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    cache[0] = 1;  cache[1] = 1;  cache[2] = 0;
    cache[3] = 1;  cache[4] = 5;  cache[5] = 10;
    cache[6] = 11;
    cache_size = base_len;
}

seq_t a(idx_t n)
{
    if (cache_size == 0) init_base();

    if (n < 7) return cache[(size_t)n];

    size_t target = (size_t)n;
    if (target >= cache_size) {
        size_t new_size = target + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        for (size_t i = cache_size; i <= target; ++i) {
            cache[i] = cache[i-3]
                     + cache[i-4] * 4
                     + cache[i-5] * 6
                     + cache[i-6] * 4
                     + cache[i-7];
        }
        cache_size = new_size;
    }
    return cache[target];
}
