/*
 * A248457  Number of length n+2 0..4 arrays with no three consecutive terms
 *          having the sum of any two elements equal to twice the third.
 * https://oeis.org/A248457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A248457:
   Number of length n+2 0..4 arrays with no three consecutive terms having
   the sum of any two elements equal to twice the third.
   Recurrence (for n >= 7):
     a(n) = 3*a(n-1) + 5*a(n-2) + 2*a(n-3)
            - 16*a(n-4) - 28*a(n-5) - 8*a(n-6)
   Initial values:
     a(1) = 96, a(2) = 380, a(3) = 1512, a(4) = 6040,
     a(5) = 24160, a(6) = 96736
   Off‑set: a(1) corresponds to index 1. */

#include <stdlib.h>   /* abort, malloc, realloc */

/* Cache of previously computed terms; indices 0..n-2 store a(1)..a(n-1). */
static seq_t *cache = NULL;
static size_t cache_sz = 0;  /* allocated size of cache */
static size_t cache_len = 0; /* number of computed terms stored */

/* Ensure that the cache can hold at least 'n' terms. */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n <= cache_sz) return;
    /* Grow to the requested size. */
    size_t new_sz = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) abort();  /* out of memory */
    cache = tmp;
    cache_sz = new_sz;
}

/* Compute a(n) using the recurrence, assuming cache contains all
   terms up to n-1. */
static seq_t compute_term(idx_t n)
{
    /* recurrence for n >= 7 */
    seq_t t1 = 3 * cache[n - 2];   /* a(n-1) */
    seq_t t2 = 5 * cache[n - 3];   /* a(n-2) */
    seq_t t3 = 2 * cache[n - 4];   /* a(n-3) */
    seq_t t4 = 16 * cache[n - 5];  /* a(n-4) */
    seq_t t5 = 28 * cache[n - 6];  /* a(n-5) */
    seq_t t6 = 8  * cache[n - 7];  /* a(n-6) */
    return t1 + t2 + t3 - t4 - t5 - t6;
}

/* Public entry point.  The function handles arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but safe */

    /* Make sure the cache can hold the requested term. */
    ensure_cache_size(n);

    /* Pre‑populate known initial values if not yet cached. */
    static const seq_t init[6] = {96, 380, 1512, 6040, 24160, 96736};

    if (cache_len == 0) {
        /* First access: fill the first six values directly. */
        for (size_t i = 0; i < 6 && i < n; ++i) {
            cache[i] = init[i];
        }
        cache_len = (size_t) ((n >= 6) ? 6 : (size_t)n);
    }

    /* Compute any missing terms up to and including n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i - 1] = init[i - 1];
        } else {
            cache[i - 1] = compute_term(i);
        }
    }
    cache_len = (size_t)n;

    return cache[n - 1];
}
