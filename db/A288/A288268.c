/*
 * A288268  Expansion of e.g.f.: exp(Sum_{k>=1} (k-1)*x^k/k).
 * https://oeis.org/A288268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=1. For n>=3:
 *            a(n)=2*(n-1)*a(n-1)-(n*n-4*n+3)*a(n-2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t capacity = 0;

seq_t a(idx_t n)
{
    /* Ensure cache storage is allocated and big enough */
    if (!cache) {
        capacity = 16;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        cache_len = 0;
    }
    if ((size_t)(n + 1) > capacity) {
        while (capacity <= (size_t)n)
            capacity <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute terms up to index n (if not already computed) */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;
        else if (i == 1)
            val = (seq_t)0;
        else if (i == 2)
            val = (seq_t)1;
        else {
            seq_t p1 = cache[i - 1];
            seq_t p2 = cache[i - 2];

            seq_t term1 = p1 * (seq_t)(2 * (i - 1));
            seq_t term2 = p2 * (seq_t)(i * i - 4 * i + 3);

            val = term1 - term2;
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
