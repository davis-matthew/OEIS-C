/*
 * A352327  Expansion of e.g.f.: 1/(3 - exp(x) - cosh(x)).
 * https://oeis.org/A352327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} ((3+(-1)^k)/2) * binomial(n,k) *
 *            a(n-k);
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   mul_small, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming with memoisation.
   All intermediate values are kept in seq_t; the recursion is
   evaluated sequentially so the cost per term is linear in n. */

#include <stddef.h>
#include <stdint.h>

/* Required helper headers (provided by the build) */
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"
#include "../utils/seq_realloc.h"

/* Cache for previously computed values. */
static seq_t *cache = NULL;   /* pointer to the cache array */
static idx_t cache_size = 0;  /* number of elements currently allocated */

/* Ensure the cache can hold values up to at least n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)            /* already large enough */
        return;

    /* Allocate a new array of size n+1, preserving old contents. */
    seq_t *new_cache = seq_realloc(cache, cache_size, n + 1);

    /* Initialise the newly allocated tail to zero. */
    for (idx_t i = cache_size; i <= n; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_size = n + 1;
}

/* Compute a(n).  The function is thread‑safe for re‑entrant
   calls only in a single‑threaded context (as the cache is static). */
seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache = seq_realloc(NULL, 0, 1);  /* size 1 for a(0) */
        cache[0] = 1;                    /* a(0) = 1 */
        cache_size = 1;
    }

    /* Determine the first index that still needs to be computed. */
    idx_t start = cache_size;          /* highest index already computed + 1 */

    /* Expand the cache if required. */
    if (n >= cache_size)
        ensure_cache(n);

    /* Compute any missing values up to n. */
    for (idx_t i = start; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;                /* binomial(i,0) = 1 */

        /* Build binomial coefficients incrementally. */
        for (idx_t k = 1; k <= i; ++k) {
            /* binom = binom * (i-k+1) / k  (exact division) */
            binom = seq_mul(binom, (seq_t)(i - k + 1));
            binom = binom / (seq_t)k;

            /* coefficient = (3 + (-1)^k) / 2  → 1 for odd k, 2 for even k */
            const unsigned int coeff = (k & 1U) ? 1U : 2U;

            /* term = coeff * binom * a(i-k) */
            seq_t term = seq_mul(cache[i - k], binom);
            term = mul_small(term, coeff);

            sum += term;
        }

        cache[i] = sum;
    }

    return cache[n];
}
