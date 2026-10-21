/*
 * A295858  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -2,
 *          a(1) = 0, a(2) = 0, a(3) = 1.
 * https://oeis.org/A295858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4); a(0) = -2, a(1)
 *            = 0, a(2) = 0, a(3) = 1
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 42, i64 n <= 88, i128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[4] = { -2, 0, 0, 1 };

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

static void ensure_cache_capacity(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    if (cache != NULL)
        return;
    ensure_cache_capacity(8);
    for (size_t i = 0; i < 4; ++i)
        cache[i] = init_vals[i];
    for (size_t i = 4; i < cache_cap; ++i)
        cache[i] = 0;
    cache_size = 4;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return init_vals[(size_t)n];

    init_cache();

    size_t un = (size_t)n;
    if (cache_size <= un) {
        ensure_cache_capacity(un + 1);
        for (size_t i = cache_size; i <= un; ++i) {
            seq_t v = cache[i-1]
                     + (seq_t)3 * cache[i-2]
                     - (seq_t)2 * cache[i-3]
                     - (seq_t)2 * cache[i-4];
            cache[i] = v;
        }
        cache_size = un + 1;
    }
    return cache[un];
}
