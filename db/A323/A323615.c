/*
 * A323615  a(0) = 1; for n > 0, a(n) = floor(a(n-1)/n) if positive and not
 *          already in the sequence, otherwise a(n) = a(n-1)*n.
 * https://oeis.org/A323615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 264, u32 n <= 4793, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A323615
 *
 *   a(0) = 1
 *   For n > 0:
 *     let q = floor(a(n‑1) / n)
 *     if q > 0 and q does NOT occur in a(0) … a(n‑2)
 *         a(n) = q
 *     else
 *         a(n) = a(n‑1) * n
 *
 *   The implementation stores previously computed terms in a
 *   dynamic array and checks membership by a linear scan of that
 *   array.  This is sufficient for n ≤ 10 000 (≈50 M
 *   comparisons).
 */
#include <stdlib.h>
#include <stdbool.h>

/* static cache of previously computed terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* Grow an array so that it can hold at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need > cache_cap) {
        size_t new_cap = (cache_cap == 0 ? 16 : cache_cap << 1);
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – terminate as the sequence cannot be
               continued safely.  The build environment will report an
               error. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the nth term of OEIS A323615. */
seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    size_t target = (size_t)n;
    if (target < cache_len)
        return cache[target];

    /* Compute terms up to target, filling the cache. */
    for (size_t idx = cache_len; idx <= target; ++idx) {
        seq_t prev = cache[idx - 1];
        seq_t q    = prev / (seq_t)idx;   /* floor division */

        bool duplicate = false;
        if (q > (seq_t)0) {
            /* Check if 'q' already appears in cache[0 .. idx-2]. */
            for (size_t j = 0; j < idx - 1; ++j) {
                if (cache[j] == q) {
                    duplicate = true;
                    break;
                }
            }
        }

        seq_t val = (q > (seq_t)0 && !duplicate) ? q : prev * (seq_t)idx;

        grow_cache(idx + 1);
        cache[idx] = val;
    }

    return cache[target];
}
