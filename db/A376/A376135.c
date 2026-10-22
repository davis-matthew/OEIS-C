/*
 * A376135  a(0) = 1; a(n) = Sum_{k=0..n-1} (-1)^k * (2*k+1) * a(k) *
 *          a(n-k-1).
 * https://oeis.org/A376135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = Σ_{k=0}^{n-1} (-1)^k * (2*k+1) * a(k) * a(n-k-1)  */
#include <stdlib.h>

/* a cache storing computed values of the sequence.
   cache_valid_len records how many elements are valid (guaranteed
   to be initialized).  The allocated capacity is cache_cap. */
static seq_t *cache = NULL;
static size_t cache_valid_len = 0;
static size_t cache_cap = 0;

/* Ensure that the cache has capacity at least cap.  If not,
   reallocate to a larger capacity. */
static void ensure_capacity(size_t cap)
{
    if (cap > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < cap) new_cap <<= 1;
        seq_t *tmp = cache ? (seq_t*)realloc(cache, new_cap * sizeof(seq_t))
                           : (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return;  /* malloc/realloc failed: leave cache unchanged */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) using the recurrence and cache. */
seq_t a(idx_t n)
{
    /* Initialise the cache for a(0) = 1 on first call. */
    if (cache_valid_len == 0) {
        ensure_capacity(1);
        if (!cache) return (seq_t)0;     /* memory allocation failed */
        cache[0] = (seq_t)1;
        cache_valid_len = 1;
    }

    /* Grow cache capacity and compute missing terms up to n. */
    if ((size_t)n >= cache_valid_len) {
        ensure_capacity((size_t)n + 1);
        if (!cache) return (seq_t)0;     /* memory allocation failed */

        for (size_t idx = cache_valid_len; idx <= (size_t)n; ++idx) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < idx; ++k) {
                seq_t prod = cache[k] * cache[idx - 1 - k];      /* a(k)*a(n-k-1) */
                prod *= (seq_t)(2 * k + 1);                     /* *(2*k+1) */
                if (k & 1) prod = -prod;                       /* (-1)^k */
                sum += prod;
            }
            cache[idx] = sum;
        }
        cache_valid_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
