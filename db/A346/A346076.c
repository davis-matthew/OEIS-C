/*
 * A346076  a(n) = 1 + Sum_{k=1..n-4} a(k) * a(n-k-4).
 * https://oeis.org/A346076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n-4} a(k) * a(n-k-4) Initial values:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=1 (offset 0)
 * Range:     u8 n <= 17, u16 n <= 29, u32 n <= 52, u64 n <= 97, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External linkage as requested.  Types seq_t and idx_t are supplied
   by the build system. */
static seq_t *cache = NULL;         /* memoised values */
static size_t cache_size = 0;       /* allocated length of cache   */
static idx_t last_computed = 0;     /* highest index computed */
static int initialized = 0;        /* flag for first call */

/* Ensure the cache holds indices 0..n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_size < needed) {
        size_t new_size = cache_size > 0 ? cache_size * 2 : needed;
        if (new_size < needed) new_size = needed;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* Initialise cache on first use. */
    if (!initialized) {
        ensure_cache_size(0);
        cache[0] = 1;
        last_computed = 0;
        initialized = 1;
    }

    /* If the value has already been computed, return immediately. */
    if (n <= last_computed) {
        return cache[n];
    }

    ensure_cache_size(n);

    /* Fill in all missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 4) {
            cache[i] = 1;
        } else {
            seq_t sum = 1;                     /* the "+1" part of the recurrence */
            idx_t limit = i - 4;
            for (idx_t k = 1; k <= limit; ++k) {
                sum += cache[k] * cache[i - k - 4];
            }
            cache[i] = sum;
        }
    }

    last_computed = n;
    return cache[n];
}
