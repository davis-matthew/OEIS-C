/*
 * A305627  a(n) = (2^n / n!) * (2^1 - 1) * (2^2 - 1) * ... * (2^n - 1).
 * https://oeis.org/A305627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n / n!) * Π_{k=1}^{n} (2^k – 1) Recurrence: a(0) = 1,
 *            a(n) = a(n‑1) * 2 * (2^n – 1) / n for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   ensure_cache, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for realloc */
#include "../utils/pow2.h"          /* pow2(idx_t) returns seq_t */
#include "../utils/ensure_cache.h"  /* ensure_cache(idx_t, seq_t**, size_t*) */

seq_t a(idx_t n) {
    static seq_t *cache = NULL; /* cached values a(0)…a(max_cached)   */
    static size_t cache_cap = 0;/* allocated capacity (number of elements)   */
    static idx_t  current_max = 0; /* highest n computed so far */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failed */
        cache[0] = (seq_t)1;
        current_max = 0;
    }

    /* Grow cache to hold index n (inclusive) */
    if ((size_t)n + 1 > cache_cap) {
        ensure_cache((idx_t)n + 1, &cache, &cache_cap);
    }

    /* Compute missing terms up to n */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        seq_t pow_i = pow2(i);                     /* 2^i                     */
        seq_t mult  = (seq_t)2 * (pow_i - (seq_t)1); /* 2 * (2^i – 1)         */
        cache[i] = cache[i - 1] * mult / (seq_t)i;   /* recurrence formula      */
    }
    if (current_max < n) current_max = n;

    return cache[n];
}
