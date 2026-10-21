/*
 * A249313  Expansion of x*(1+13*x-12*x^3)/(1-14*x^2+12*x^4).
 * https://oeis.org/A249313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-2) - 12*a(n-4) Initial values: a(1)=1, a(2)=13,
 *            a(3)=14, a(4)=170
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_len)
        return;
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    cache = new_cache;

    if (cache_len < 4) {
        size_t upto = new_len < 4 ? new_len : 4;
        if (upto >= 1) cache[1] = (seq_t)1;
        if (upto >= 2) cache[2] = (seq_t)13;
        if (upto >= 3) cache[3] = (seq_t)14;
        if (upto >= 4) cache[4] = (seq_t)170;
        for (idx_t i = 5; i <= new_len; ++i) {
            cache[i] = (seq_t)((seq_t)14 * cache[i - 2]
                               - (seq_t)12 * cache[i - 4]);
        }
    } else {
        for (idx_t i = cache_len + 1; i <= new_len; ++i) {
            cache[i] = (seq_t)((seq_t)14 * cache[i - 2]
                               - (seq_t)12 * cache[i - 4]);
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
