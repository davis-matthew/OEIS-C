/*
 * A265905  a(1) = 1; for n > 1, a(n) = a(n-1) + A153880(a(n-1)).
 * https://oeis.org/A265905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) + A153880(a(n-1)). A153880(x)
 *            = sum_{k=1..m} d_k * (k+1)! where d_k are the factorial‑base
 *            digits of x
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   A153880
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/A153880.h"

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored (a(1) at index 0) */
static size_t cache_cap = 0;   /* allocated capacity */

/* ensure the cache contains terms up to 'n' (1‑based indexing) */
static void ensure_cache(idx_t n)
{
    /* n is at least 1 */
    if (n == 0) return;

    /* first term a(1) */
    if (cache_len == 0) {
        /* allocate space for at least one term */
        if (cache_cap < 1) {
            cache_cap = 1;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        cache[0] = (seq_t)1;      /* a(1) */
        cache_len = 1;
    }

    /* grow capacity if necessary */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];           /* a(i) = a(i-1) + A153880(a(i-1)) */
        seq_t added = A153880(prev);
        cache[i] = prev + added;
    }
    cache_len = (size_t)n;
}

/* PUBLIC API ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* out of bounds, not used */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
