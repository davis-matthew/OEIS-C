/*
 * A376126  a(n) = 1 + Sum_{k=0..n-1} (k+1)^2 * a(k) * a(n-k-1).
 * https://oeis.org/A376126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} (k+1)^2 * a(k) * a(n-k-1) (OEIS
 *            A376126)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* memoization array */
static size_t cap = 0;           /* allocated capacity */
static size_t computed_len = 0;  /* number of terms already computed (first term a(0) at index 0) */

/* Ensure the cache can store up to (n+1) terms.  Newly allocated memory is zero‑initialised. */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cap)
        return;
    size_t new_cap = cap ? cap * 2 : 64;
    while (new_cap < required)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – terminate program (not expected in tests). */
        abort();
    }
    /* Zero initialise the newly allocated part. */
    memset((void *)(new_cache + cap), 0, (new_cap - cap) * sizeof(seq_t));
    cache = new_cache;
    cap = new_cap;
}

/* Compute a(n) using the defining recurrence with memoisation. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    /* Compute any missing terms up to n. */
    for (idx_t m = (idx_t)computed_len; m <= n; ++m) {
        if (m == 0) {
            cache[0] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)1;
            for (idx_t k = 0; k < m; ++k) {
                seq_t term = (seq_t)(k + 1) * (seq_t)(k + 1); /* (k+1)^2 */
                term *= cache[k];
                term *= cache[m - k - 1];
                sum += term;
            }
            cache[m] = sum;
        }
    }
    computed_len = (size_t)n + 1;
    return cache[n];
}
