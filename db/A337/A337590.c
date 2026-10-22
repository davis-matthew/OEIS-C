/*
 * A337590  a(0) = 0; a(n) = n - (1/n) * Sum_{k=1..n-1} binomial(n,k)^2 *
 *          (n-k) * k * a(k).
 * https://oeis.org/A337590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A337590
 * Recurrence:
 *   a(0) = 0
 *   a(1) = 1
 *   for n > 1:
 *     a(n) = n - (1/n) * Σ_{k=1}^{n-1} binomial(n,k)^2 * (n-k) * k * a(k)
 *
 * This implementation memoises the values of a(k) and computes
 * the binomial coefficients iteratively to avoid factorials.
 *
 * It works for any seq_t type supplied by the build system
 * (int8_t, int16_t, …, __int128, or an arbitrary‑precision class)
 * and uses the built‑in idx_t (uint64_t) for indices.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* The build supplies seq_t and idx_t.  Do not define them here. */

static seq_t *cache = NULL;       /* memoised a(k) values */
static size_t cache_len = 0;      /* allocated number of slots */
static idx_t  last_computed = 0;  /* largest n with a(n) stored */

/* Ensure the cache can hold indices up to `n`.  Newly allocated entries
 * are zeroed by calloc.  Any reallocation preserves existing values.
 */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_len)
        return;

    seq_t *new_cache = (seq_t *)calloc(required, sizeof(seq_t));
    if (new_cache == NULL)
        abort();   /* allocation failure */

    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache      = new_cache;
    cache_len  = required;
}

/* Compute a(n) for any n >= 0.  The function is thread‑unsafe but
 * provides memoised values so repeated calls are fast.
 */
seq_t a(idx_t n)
{
    /* Initialise base values lazily.  cache[0] = 0, cache[1] = 1. */
    if (cache_len == 0) {
        ensure_cache(1);      /* space for indices 0 and 1 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* Make sure the cache has room for the requested index. */
    ensure_cache(n);

    /* Compute any missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* For i = 1 we already have the value; skip. */
        if (i == 0) { cache[0] = (seq_t)0; continue; }
        if (i == 1) { cache[1] = (seq_t)1; continue; }

        seq_t sum = (seq_t)0;
        seq_t C   = (seq_t)1;      /* binomial(i,0) = 1 */

        for (idx_t k = 1; k <= i - 1; ++k) {
            /* Update C to binomial(i, k) using C(i,k) = C(i,k-1)*(i-k+1)/k */
            C = (C * (seq_t)(i - k + 1)) / (seq_t)k;

            /* term = C^2 * (i-k) * k * a(k) */
            seq_t term = C * C;
            term *= (seq_t)((i - k) * k);
            term *= cache[(size_t)k];

            sum += term;
        }

        seq_t i_seq = (seq_t)i;
        /* a(i) = i - sum / i  (all integer operations) */
        cache[(size_t)i] = i_seq - sum / i_seq;
    }

    last_computed = n;
    return cache[(size_t)n];
}
