/*
 * A355253  Expansion of e.g.f. exp(2*(exp(x) - 1) - 3*x).
 * https://oeis.org/A355253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -3*a(n-1) + 2* Σ_{k=1..n} binomial(n-1,k-1)*a(n-k);
 *            Initial value: a(0) = 1
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 15, i64 n <= 24, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;             /* array holding computed values */
static size_t cache_cap = 0;            /* current allocated capacity */
static size_t last_computed = 0;        /* highest index computed so far */

/* Ensure the cache can store indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    seq_t *new_cache = (seq_t *)calloc(needed, sizeof(seq_t));
    if (!new_cache) abort();            /* out of memory */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = needed;
    /* On first allocation set a(0)=1 */
    if (cache_cap > 0 && cache[0] == 0)
        cache[0] = (seq_t)1;
}

/* Compute the sequence value a(n). */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* If we already have the value, just return it. */
    if ((size_t)n <= last_computed)
        return cache[(size_t)n];

    /* Compute values for all indices up to n that are not yet known. */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        /* Compute Σ_{j=0..i-1} binomial(i-1,j)*a(i-1-j) */
        seq_t sum  = 0;
        seq_t bin  = 1;                 /* binomial(i-1,0) */

        for (size_t j = 0; j < i; ++j) {
            size_t idx = i - 1 - j;     /* a(i-1-j) */
            seq_t term = bin * cache[idx];
            sum += term;

            /* Update binomial(i-1,j+1) = bin * (i-1-j)/(j+1) */
            if (j + 1 < i) {
                bin *= (seq_t)(i - 1 - j);
                bin /= (seq_t)(j + 1);
            }
        }

        cache[i] = -3 * cache[i - 1] + 2 * sum;
    }

    last_computed = (size_t)n;
    return cache[(size_t)n];
}
