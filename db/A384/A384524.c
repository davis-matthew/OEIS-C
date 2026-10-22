/*
 * A384524  Expansion of e.g.f. 2/(7 - 5*exp(2*x)).
 * https://oeis.org/A384524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 * Σ_{k=1..n} 2^(k-1) * binomial(n,k) * a(n-k), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
/* seq_t and idx_t are provided by the build.  No other headers are required. */

static seq_t *cache = NULL;        /* dynamic array holding a(0)..a(n) */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t  cache_compute_up_to = 0;   /* highest index that has been computed */

/* Ensure that the cache has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        /* cast is required for C++ compatibility */
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        cache = tmp;
        cache_cap = need;
    }
}

/* Return the n-th term of OEIS A384524.  The function is memoised so that
   each value is computed only once, regardless of call order. */
seq_t a(idx_t n)
{
    /* Base value */
    if (n == 0)
        return (seq_t)1;

    /* Initialise cache on first call */
    if (cache == NULL) {
        ensure_cache(0);
        cache[0] = (seq_t)1;
        cache_compute_up_to = 0;
    }

    /* Return cached value if already known */
    if (n <= cache_compute_up_to)
        return cache[n];

    /* Make room for the new values */
    if ((size_t)n >= cache_cap)
        ensure_cache(n);

    /* Compute all missing terms up to n */
    for (idx_t m = cache_compute_up_to + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t pow2 = (seq_t)1;          /* 2^(k-1) for current k */
        seq_t bin  = (seq_t)1;          /* binomial(m,0) */

        for (idx_t k = 1; k <= m; ++k) {
            /* Update bin to binomial(m,k) */
            bin = bin * (seq_t)(m - k + 1);   /* multiply by numerator */
            bin = bin / (seq_t)k;             /* divide by denominator */

            /* term = bin * a(m-k) * 5 * 2^(k-1) */
            seq_t term = bin * cache[m - k];
            term = term * (seq_t)5;
            term = term * pow2;

            sum = sum + term;

            /* Advance pow2 to 2^k for next iteration */
            pow2 = pow2 * (seq_t)2;
        }
        cache[m] = sum;
    }

    cache_compute_up_to = n;
    return cache[n];
}
