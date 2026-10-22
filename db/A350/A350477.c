/*
 * A350477  G.f. A(x) satisfies: A(x) = A(x^2 - x^5)/x.
 * https://oeis.org/A350477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1 For n ≥ 2: a(n) = Σ_{m=ceil((n+3)/5)}^{floor((n+1)/2)}
 *            a(m) * (-1)^{n+1-2m} * binom(3m-2, n+1-2m) where binom(α,β) is
 *            the binomial coefficient. The terms are indexed from 1 as in the
 *            OEIS entry
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 30, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;        /* array holding computed a(n)   */
static size_t cache_cap = 0;       /* allocated capacity of cache   */
static idx_t  cache_used = 0;      /* number of terms already computed */

/* Ensure the cache can store at least n terms (n ≥ 1).  New slots are
   left zero‑initialised; existing computed values are preserved.     */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;
    size_t new_cap = (size_t)n;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return;                     /* allocation failure – caller will fail */
    if (cache) {
        memcpy(new_cache, cache, cache_used * sizeof(seq_t));
        free(cache);
    }
    /* zero‑initialise the new part */
    memset(new_cache + cache_used, 0, (new_cap - cache_used) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of sequence A350477 (1‑based indexing).      */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined, but harmless */

    /* Initialise cache with first term if necessary. */
    if (cache_used == 0) {
        ensure_cache(1);
        cache[0] = 1;               /* a(1) = 1                    */
        cache_used = 1;
    }

    /* Return already computed value. */
    if (n <= cache_used)
        return cache[n - 1];

    /* Make room for all required terms. */
    ensure_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t idx = cache_used; idx < n; ++idx) {
        idx_t N = idx + 1;           /* current index (1‑based)       */
        seq_t sum = 0;

        idx_t m_min = (N + 7) / 5;    /* ceil((N+3)/5)                */
        idx_t m_max = (N + 1) / 2;    /* floor((N+1)/2)                */

        for (idx_t m = m_min; m <= m_max; ++m) {
            idx_t j = N - 2 * m + 1;   /* n+1-2m                       */
            seq_t coef = binom(3 * m - 2, j);
            if (j & 1)                /* multiply by (-1)^j           */
                coef = -coef;
            sum += cache[m - 1] * coef;
        }
        cache[idx] = sum;           /* store a(N)                    */
    }
    cache_used = n;
    return cache[n - 1];
}
