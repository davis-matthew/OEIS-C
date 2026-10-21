/*
 * A258451  a(n) = 1 + a(n-1)/gcd(a(n-1),n) with a(0)=3.
 * https://oeis.org/A258451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 3; a(n) = 1 + a(n-1)/gcd(a(n-1), n) for n >= 1
 * Range:     u8 n <= 720, u16 n <= 9497, u32 n <= 9497, u64 n <= 9497,
 *            u128 n <= 9497, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;  /* number of elements computed */

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;  /* safe under the 10 000 limit */
    if (cache_len == 0) {    /* initialise a(0) */
        ensure_cache_cap(1);
        cache[0] = (seq_t)3;
        cache_len = 1;
    }
    if (idx >= cache_len) {
        ensure_cache_cap(idx + 1);
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t prev = cache[i - 1];
            seq_t g = gcd(prev, (seq_t)i);
            cache[i] = (seq_t)1 + prev / g;
        }
        cache_len = idx + 1;
    }
    return cache[idx];
}
