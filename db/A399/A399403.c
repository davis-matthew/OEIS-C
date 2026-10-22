/*
 * A399403  G.f. A(x) satisfies: A(x) = x - (1/2) * ( A(x)^2 + A(x^2) ).
 * https://oeis.org/A399403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   the ordinary generating function satisfies A(x) = x - (1/2) * (
 *            A(x)^2 + A(x^2) ). Recurrence (derived by coefficient
 *            comparison): a(0) = 0 a(1) = 1 for n ≥ 2: a(n) = - (
 *            Σ_{k=1}^{n-1} a(k)·a(n-k) + a(n/2) (if n even) ) / 2 This
 *            recurrence uses only integer arithmetic; all intermediate values
 *            are...
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 31, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#ifndef NO_ALLOC
/* Allocate an array of seq_t of given length.  Uses C allocation in C,
   operator new in C++ and performs zero‑initialisation. */
#ifdef __cplusplus
static seq_t *alloc_seq_array(size_t n)
{
    seq_t *p = new seq_t[n]();            /* default‑initialise */
    return p;
}
static void free_seq_array(seq_t *p)
{
    delete[] p;
}
#else
static seq_t *alloc_seq_array(size_t n)
{
    seq_t *p = (seq_t *)malloc(n * sizeof(seq_t));
    if (!p) abort();
    for (size_t i = 0; i < n; ++i) p[i] = 0;
    return p;
}
static void free_seq_array(seq_t *p)
{
    free(p);
}
#endif
#endif /* NO_ALLOC */

/* Memoised cache of computed terms. */
static seq_t  *seq_cache   = (seq_t *)0;
static size_t cache_len    = 0;   /* number of valid terms a[0]..a[cache_len-1] */
static size_t cache_cap    = 0;   /* allocated capacity */

/* Ensure the cache can hold at least 'needed' terms.  Expands as required. */
static void
ensure_cache(size_t needed)
{
    if (seq_cache != (seq_t *)0 && needed <= cache_cap)
        return;

    /* Grow the cache to 'needed' elements. */
    seq_t *new_cache = alloc_seq_array(needed);
    if (seq_cache != (seq_t *)0) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = seq_cache[i];   /* copy existing terms */
        free_seq_array(seq_cache);
    }
    seq_cache = new_cache;
    cache_cap = needed;
    if (cache_len == 0) {
        seq_cache[0] = 0;          /* a(0) = 0 */
        cache_len   = 1;
    }
}

/* Main entry point.  Computes a(n) for arbitrary n ≥ 0, in any call order. */
seq_t a(idx_t n)
{
    /* Convert idx_t to size_t safely (n is guaranteed non‑negative). */
    size_t target = (size_t)n;
    ensure_cache(target + 1);

    /* Compute all missing terms up to n. */
    for (size_t m = cache_len; m <= target; ++m) {
        if (m == 1) {                       /* a(1) = 1 */
            seq_cache[1] = 1;
            continue;
        }
        seq_t convSum = 0;
        for (size_t k = 1; k < m; ++k)
            convSum += seq_cache[k] * seq_cache[m - k];

        seq_t halfTerm = (m & 1U) ? 0 : seq_cache[m / 2];
        seq_t tmp = -(convSum + halfTerm);
        seq_cache[m] = tmp / 2;             /* division is exact */
    }

    cache_len = target + 1;
    return seq_cache[target];
}
