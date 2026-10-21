/*
 * A254708  Expansion of (1 + 2*x^2) / (1 - 2*x^2 - x^3 + 2*x^5 + 2*x^6 - x^8
 *          - 2*x^9 + x^11) in powers of x.
 * https://oeis.org/A254708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 0, a(2) = 5, a(3) = 2, a(4) = 10, a(5) = 5,
 *            a(6) = 18, a(7) = 10, a(8) = 29, a(9) = 18. For n ≥ 10: a(n) = 3
 *            + a(n‑10)+a(n‑9) - a(n‑8) - 2*a(n‑7) - 2*a(n‑6) + 2*a(n‑4) +
 *            2*a(n‑3) + a(n‑2) - a(n‑1)
 * Range:     u8 n <= 23, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initial seed values */
    static const seq_t seeds[10] = {
        2, 0, 5, 2, 10,
        5, 18,10,29,18
    };

    /* On first call: allocate at least 10 entries and fill seeds */
    if (cache == NULL) {
        size_t init_cap = 10;
        cache_cap = init_cap;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = seeds[i];
        cache_len = 10;
    }

    /* Ensure capacity up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If needed, compute missing terms up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 10) {
                /* seeds already present */
                cache[i] = seeds[i];
            } else {
                /* Compute using the recurrence */
                seq_t pos = cache[i-10] + cache[i-9]
                          + cache[i-2] + (cache[i-4] << 1) + (cache[i-3] << 1);
                seq_t neg = cache[i-8] + (cache[i-7] << 1) + (cache[i-6] << 1) + cache[i-1];
                cache[i] = pos - neg + (seq_t)3;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
