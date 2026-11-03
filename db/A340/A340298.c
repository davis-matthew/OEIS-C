/*
 * A340298  a(n) = a(floor(n/2)) + a(ceiling(n/2)) + n*floor(log_2(n)) for n
 *          >= 2, a(n) = n for n <= 1.
 * https://oeis.org/A340298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(floor(n/2)) + a(ceil(n/2)) + n*floor(log_2(n)), a(0) =
 *            0, a(1) = 1
 * Range:     u8 n <= 22, u16 n <= 1165, u32 n <= 10000, u64 n <= 10000,
 *            u128 none, big any n
 * Validated: standard, pass, 72 terms through n = 9813 (2026-11-03)
 * Helpers:   ilog2, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ilog2.h"
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold indices up to n.  Newly created entries
   are initialized to zero (valid for a(n) > 0 when n > 0). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)(n + 1);
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 1;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *new_cache = alloc_seq_array(new_cap);
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free_seq(cache);
    }
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;      /* zero is the default “not computed” marker */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Recursive helper that uses the cache. */
static seq_t compute_val(idx_t n)
{
    if (n <= 1)            /* a(0) = 0, a(1) = 1 */
        return (seq_t)n;

    if (cache[n] != 0)     /* already computed */
        return cache[n];

    idx_t half = n >> 1;           /* floor(n/2) */
    idx_t ceil = n - half;         /* ceil(n/2) */
    seq_t left  = compute_val(half);
    seq_t right = compute_val(ceil);

    unsigned log2 = ilog2(n);
    seq_t add = (seq_t)n * (seq_t)log2;
    seq_t val = left + right + add;

    cache[n] = val;
    return val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    cache[0] = 0;
    cache[1] = 1;
    return compute_val(n);
}
