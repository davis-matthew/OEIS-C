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
 * Validated: standard, pass, 33 terms through n = 75 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for realloc */
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL; /* cached values a(0)…a(max_cached)   */
    static size_t cache_cap = 0;/* allocated capacity (number of elements)   */
    static idx_t  current_max = 0; /* highest n computed so far */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 1;
        cache = mem_resize(seq_t, NULL, cache_cap);
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
