/*
 * A285588  a(1) = a(2) = 1; a(3) = a(4) = a(5) = 2; a(n) = a(a(n-3)-1) +
 *          a(n-a(n-5)-2) for n > 5.
 * https://oeis.org/A285588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 773, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(1) = a(2) = 1;
 *   a(3) = a(4) = a(5) = 2;
 *   a(n) = a(a(n-3)-1) + a(n-a(n-5)-2)   for n > 5
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache for already computed values.  cache[0] is unused to keep indices
 * 1‑based.  The array is lazily allocated and expanded. */
static seq_t *cache          = NULL;   /* 1‑based array of seq_t */
static size_t cache_cap      = 0;      /* allocated size (including index 0) */
static idx_t  cache_computed = 0;      /* largest n computed (cache_computed >= 0) */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void ensure_cache_cap(idx_t n)
{
    /* In terms of array size we need n+1 elements (0..n). */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;          /* double until enough */

    if (!cache) {
        /* Allocate new cache.  All slots are uninitialized but will be
         * filled before first use. */
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache_cap = new_cap;
}

/* Compute all terms up to n (inclusive) and store them in cache. */
static void compute_upto(idx_t n)
{
    if (n <= cache_computed)
        return;

    ensure_cache_cap(n);

    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = (seq_t)1;
        } else if (i <= 5) {
            cache[i] = (seq_t)2;
        } else {
            /* a(i-3)-1  (converted to idx_t) */
            seq_t val_a   = cache[i - 3];
            idx_t idx1 = (idx_t)val_a;
            --idx1;  /* a(i-3)-1 */

            /* i - a(i-5) - 2  (converted to idx_t) */
            seq_t val_b   = cache[i - 5];
            idx_t idx2 = i - (idx_t)val_b - 2;

            /* By definition of the sequence these indices are >= 1. */
            cache[i] = cache[idx1] + cache[idx2];
        }
    }
    cache_computed = n;
}

/* Public function returning the nth term (1‑based index). */
seq_t a(idx_t n)
{
    /* The specification says that behavior for n < 1 is irrelevant. */
    if (n < 1)
        return 0;
    compute_upto(n);
    return cache[n];
}
