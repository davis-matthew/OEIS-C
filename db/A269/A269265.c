/*
 * A269265  a(0) = a(1) = 1; thereafter a(n) = a(n-1) + a(n-2) if n is even,
 *          otherwise a(n) = a(n-1)^2.
 * https://oeis.org/A269265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1; for n>1: if n even, a(n)=a(n-1)+a(n-2); else
 *            a(n)=a(n-1)^2
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_comp = (idx_t)-1;

    /* Initialize cache on first call */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        max_comp = 1;
    }

    /* Return cached value if already computed */
    if (n <= max_comp)
        return cache[(size_t)n];

    /* Extend capacity if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute successive terms */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i & 1U) { /* odd index: square previous term */
            cache[(size_t)i] = cache[(size_t)(i - 1)] * cache[(size_t)(i - 1)];
        } else { /* even index: sum of previous two terms */
            cache[(size_t)i] = cache[(size_t)(i - 1)] + cache[(size_t)(i - 2)];
        }
    }
    max_comp = n;
    return cache[(size_t)n];
}
