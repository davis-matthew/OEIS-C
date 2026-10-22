/*
 * A360483  E.g.f. satisfies A(x) = x * exp(x - 2 * A(x)).
 * https://oeis.org/A360483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1..n} (-2)^(k-1) * k^(n‑1) * binomial(n,k)
 *            Recurrence: a(0)=0, a(1)=1, for n≥2 a(n) = n/(n‑1) * a(n‑1) –
 *            2*n/(n‑1) * Σ_{i=1}^{n‑1} binom(n‑1,i‑1) * a(i) * a(n‑i)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 17,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache of already computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* size of the allocated array   */
static idx_t  cache_max = 0;   /* highest index that has been computed */

/* Ensure the cache has room for indices up to n (inclusive) */
static void
ensure_cache_size(idx_t n)
{
    if (cache_cap > (size_t)n) return;      /* already big enough   */
    size_t new_cap = (size_t)n + 1;          /* need indices 0..n    */

    if (cache == NULL) {
        cache = seq_alloc(new_cap);
    } else {
        cache = seq_realloc(cache, cache_cap, new_cap);
    }

    /* Zero any newly allocated elements */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        cache[i] = (seq_t)0;
    }

    cache_cap = new_cap;
}

/* Initialise cache with the two base values a(0)=0, a(1)=1 */
static void
init_cache(void)
{
    ensure_cache_size(1);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache_max = 1;
}

/* Compute all terms up to and including n (inclusive) */
static void
compute_up_to(idx_t n)
{
    if (cache_max == 0) init_cache();   /* first call          */
    if (n <= cache_max) return;         /* already known       */

    ensure_cache_size(n);

    for (idx_t cur = cache_max + 1; cur <= n; ++cur) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;   /* binom(cur-1,0) */

        for (idx_t i = 1; i < cur; ++i) {
            seq_t term = cache[i] * cache[cur - i];
            term *= binom;
            sum += term;

            if (i < cur - 1) {
                seq_t num = (seq_t)(cur - i);
                seq_t den = (seq_t)i;
                binom = binom * num / den;   /* next binomial */
            }
        }

        seq_t term1 = cache[cur - 1] * (seq_t)cur;
        term1 /= (seq_t)(cur - 1);

        seq_t term2 = sum * (seq_t)(2 * cur);
        term2 /= (seq_t)(cur - 1);

        cache[cur] = term1 - term2;
    }

    cache_max = n;
}

/* Public API function required by the build system. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
