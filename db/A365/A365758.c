/*
 * A365758  G.f. satisfies A(x) = 1 + x*A(x) / (1 - x^4*A(x)^5).
 * https://oeis.org/A365758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(n-3k-1,k) *
 *            binomial(n+k+1,n-4k) / (n+k+1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

/* Memoised storage of computed terms */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_size = 0;        /* number of terms already stored */

/* Ensure that the cache can hold at least `needed` elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;               /* double until enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {                      /* allocation failed */
        /* In a real project we would handle this properly */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the nth term of OEIS sequence A365758 */
seq_t a(idx_t n)
{
    /* Make sure we have room for n+1 terms */
    ensure_cache_capacity((size_t)n + 1);

    /* If already computed, return the cached value */
    if (cache_size > n)
        return cache[n];

    /* Compute all terms from the current end up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {               /* a(0) = 1 by definition */
            cache[0] = (seq_t)1;
            continue;
        }

        idx_t maxk = i / 4;
        seq_t sum = 0;

        for (idx_t k = 0; k <= maxk; ++k) {
            /* binomial(n-3k-1,k) – handle k==0 separately to avoid underflow */
            seq_t t1 = (k == 0) ? (seq_t)1
                                 : binom(i - 3 * k - 1, k);

            /* binomial(n+k+1,n-4k) */
            seq_t t2 = binom(i + k + 1, i - 4 * k);

            /* denominator (n+k+1) */
            seq_t denom = (seq_t)(i + k + 1);

            /* term = t1 * t2 / denom */
            seq_t term = t1 * t2 / denom;

            sum += term;
        }

        cache[i] = sum;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
