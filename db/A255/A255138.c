/*
 * A255138  a(n) = (1 + 2^n*(3 + 2*(-1)^n))/3.
 * https://oeis.org/A255138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2^n*(3 + 2*(-1)^n))/3; Recurrence: a(0)=2, a(1)=1,
 *            a(n)=4*a(n-2)-1 for n≥2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache = (seq_t*)calloc(2, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache_cap = 2;
    }
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cache_cap; i <= (size_t)n; ++i) {
            /* recurrence: a(i) = 4*a(i-2) - 1   (valid for i ≥ 2) */
            cache[i] = cache[i - 2] * (seq_t)4 - (seq_t)1;
        }
        cache_cap = new_cap;
    }
    return cache[(size_t)n];
}
