/*
 * A375285  Expansion of 1/((1 - x - x^5)^2 - 4*x^6).
 * https://oeis.org/A375285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-5) + 2*a(n-6) - a(n-10), with
 *            initial values a(0) = 1, a(1) = 2, a(2) = 3, a(3) = 4, a(4) = 5,
 *            a(5) = 8, a(6) = 17, a(7) = 36, a(8) = 69, a(9) = 120
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Lazy static initialization of base terms */
    if (cache_len == 0) {
        /* Allocate space for the initial 10 values plus room for growth */
        ensure_capacity(16);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)8;
        cache[6] = (seq_t)17;
        cache[7] = (seq_t)36;
        cache[8] = (seq_t)69;
        cache[9] = (seq_t)120;
        cache_len = 10;
    }

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute terms up to n */
    ensure_capacity((size_t)n + 1);
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = 2 * cache[i - 1] - cache[i - 2]
                    + 2 * cache[i - 5] + 2 * cache[i - 6]
                    - cache[i - 10];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
