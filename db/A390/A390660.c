/*
 * A390660  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - 2 * x * A(x^3))).
 * https://oeis.org/A390660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2 * Σ_{k=0..⌊(n-1)/3⌋} a(k) * a(n-1-3*k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence with a(0) = 1 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"  /* sequence allocator helper */

static seq_t *seq_cache = NULL;   /* array of computed values          */
static size_t cache_cap = 0;      /* allocated capacity                */
static size_t cache_len = 0;      /* number of values already computed */

/* Ensure the cache has capacity for at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;          /* double until big enough */

    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (seq_cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = seq_cache[i];
        free(seq_cache);
    }
    seq_cache = new_cache;
    cache_cap = new_cap;
}

/* Compute all a(i) for i from current length up to n. */
static void compute_up_to(idx_t n)
{
    if (n + 1U <= cache_len)           /* already computed up to n */
        return;

    if (cache_len == 0) {              /* initialise a(0) */
        ensure_capacity(1);
        seq_cache[0] = (seq_t)1;
        cache_len = 1;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        ensure_capacity(i + 1);        /* at least i+1 slots */

        /* Recurrence: a(n) = 1 + 2 * Σ a(k) * a(n-1-3k)  */
        seq_t sum = 0;
        idx_t limit = (i == 0) ? 0U : (i - 1) / 3;
        for (idx_t k = 0; k <= limit; ++k) {
            seq_t prod = seq_cache[k] * seq_cache[i - 1 - 3 * k];
            sum += prod;
        }
        sum = sum * 2;
        sum += 1;

        seq_cache[i] = sum;
        cache_len = i + 1;             /* update count after storing */
    }
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return seq_cache[n];
}
