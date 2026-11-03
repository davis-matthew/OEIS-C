/*
 * A386454  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} binomial(k+3,4) * a(k) *
 *          a(n-1-k).
 * https://oeis.org/A386454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Sum_{k=0..n-1} binomial(k+3,4) * a(k) *
 *            a(n-1-k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Binomial coefficient (k+3 choose 4) implemented directly to avoid external dependency. */
static seq_t binom_k3_4(idx_t k)
{
    seq_t num = (seq_t)(k + 3) * (k + 2) * (k + 1) * k;
    return num / 24;
}

/* Cache for computed terms.  cache_len indicates the first unused index. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* allocated slots */
static size_t cache_len   = 0;   /* next index to compute */

/* Return a(n) for the OEIS sequence A386454.  Results are memoized so that
 * any order of calls is supported. */
seq_t a(idx_t n)
{
    /* Initial allocation for base case a(0). */
    if (cache_size == 0) {
        cache_size = 1;
        cache = mem_new(seq_t, cache_size);
        if (!cache) return 0;              /* allocation failure */
        mem_set(cache, 0, sizeof(seq_t) * cache_size);
    }

    /* Expand cache if necessary to accommodate n. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) return 0;                /* allocation failure */
        mem_set(tmp + cache_size, 0, sizeof(seq_t) * (new_size - cache_size));
        cache = tmp;
        cache_size = new_size;
    }

    /* Ensure a(0) is set. */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* Compute missing terms up to and including n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = cache[i - 1];         /* a(i-1) */
        for (idx_t k = 0; k < i; ++k) {
            seq_t coeff = binom_k3_4(k);
            sum += coeff * cache[k] * cache[i - 1 - k];
        }
        cache[i] = sum;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
