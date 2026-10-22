/*
 * A367718  G.f. A(x) satisfies A(x) = 1 / ((1 + x) * (1 - x * (1 + x + x^2 +
 *          x^3) * A(x^4))).
 * https://oeis.org/A367718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=0..n-1} a(floor(k/4)) * a(n-1-k)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The function fills a static cache, so any order of calls works.
   It uses O(n^2) arithmetic operations, which is fast enough for n ≤ 10000
   with the usual integer or arbitrary‑precision backends. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;   /* highest n in cache */

    /* Ensure cache allocated for current n */
    if (!initialized) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* fallback on allocation failure */
        cache[0] = 1;                  /* a(0) = 1 */
        cache_cap = 1;
        initialized = true;
    }
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* fallback on allocation failure */
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += cache[k / 4] * cache[i - 1 - k];
        }
        /* Add (-1)^i: (+1 for even i, -1 for odd i) */
        if (i & 1) {
            /* i is odd → subtract 1 */
            sum -= 1;
        } else {
            /* i is even → add 1 */
            sum += 1;
        }
        cache[i] = sum;
    }
    last_computed = n;

    return cache[n];
}
