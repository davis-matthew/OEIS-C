/*
 * A290770  a(n) = Product_{k=1..n} k^(2*k).
 * https://oeis.org/A290770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} k^(2*k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 27 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;     /* array of a(i) */
    static size_t alloced = 0;      /* allocated array size (>= n+1) */
    static idx_t  computed = 0;     /* largest index whose value is stored */

    /* Initialise on first call */
    if (cache == NULL) {
        alloced = (size_t)n + 1;
        if (alloced < 1) alloced = 1;
        cache = mem_new(seq_t, alloced);
        if (!cache) return (seq_t)0;      /* out‑of‑memory; arbitrary result */
        cache[0] = (seq_t)1;              /* a(0) = 1 */
        computed = 0;
    }
    /* Grow the cache if needed */
    if ((size_t)n + 1 > alloced) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return (seq_t)0;        /* out‑of‑memory; arbitrary result */
        cache = tmp;
        alloced = new_cap;
    }
    /* Compute any missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t pow = ipow((seq_t)i, (seq_t)(2 * i));   /* i^(2i) */
        cache[i] = cache[i - 1] * pow;
    }
    computed = n;
    return cache[n];
}
