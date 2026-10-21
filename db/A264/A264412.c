/*
 * A264412  G.f. A(x) satisfies: A(x)^2 = A(x^2) + 6*x.
 * https://oeis.org/A264412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 29, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A264412
 *
 * Recurrence derived from A(x)^2 = A(x^2) + 6*x (offset 0):
 *   a(0) = 1, a(1) = 3
 *   for n ≥ 2
 *     let S = Σ_{i=1}^{n-1} a(i) * a(n-i)
 *     if n is even: a(n) = (a(n/2) - S) / 2
 *     if n is odd : a(n) = (-S) / 2
 *   (The division by 2 is exact.)
 *
 * The sequence values are cached in a static array and computed on demand,
 * which allows arbitrary order of calls.  The implementation uses only
 * the operations allowed on seq_t and idx_t and is valid C11 and C++17.
 */

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

/* Maximum number of values that can be computed.  200 000 covers
 * n up to about 100 000 comfortably.  Increase if required. */
#define MAX_CACHE 200000

/* Static cache of computed terms */
static seq_t cache[MAX_CACHE];
static idx_t cache_len = 0;   /* number of cached terms */

/* Ensure that the cache contains values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;          /* already cached */

    /* Initialise the first two terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* Make sure we have enough space */
    assert((idx_t)MAX_CACHE > n);  /* n must be less than MAX_CACHE */

    /* Compute missing terms */
    for (idx_t k = cache_len; k <= n; ++k) {
        if (k == 0) { cache[0] = (seq_t)1; continue; }
        if (k == 1) { cache[1] = (seq_t)3; continue; }

        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i) {
            sum += cache[i] * cache[k - i];
        }

        seq_t term;
        if ((k & 1) == 0) {            /* k even */
            term = cache[k >> 1] - sum;
        } else {                       /* k odd */
            term = -sum;
        }

        /* Division by 2 is exact; truncation works for negative numbers too */
        cache[k] = term / 2;
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    /* Compute all terms up to n if necessary, then return the result */
    ensure_cache(n);
    return cache[n];
}
