/*
 * A369756  Expansion of e.g.f. exp( (1 - (1+x)^5)/5 ).
 * https://oeis.org/A369756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; for n>=1, a(n) = -( a(n-1) + 4*a(n-2)*(n-1) +
 *            6*a(n-3)*(n-1)*(n-2) + 4*a(n-4)*(n-1)*(n-2)*(n-3) +
 *            a(n-5)*(n-1)*(n-2)*(n-3)*(n-4) ); (minimize to the available
 *            terms when n<5)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoization array */
static size_t  cap   = 0;        /* allocated size of cache       */
static idx_t   computed_max = 0; /* highest index already computed */

/* Ensure that cache can hold values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cap <= (size_t)n) {
        size_t new_cap = (cap == 0) ? 1 : cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        /* initialize new entries to zero */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }
    /* always set a[0] = 1 */
    cache[0] = (seq_t)1;
}

/* Compute a(n) using the recurrence and memoization. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Compute values only once, expanding the cache as needed. */
    if (computed_max < n) {
        /* binomial coefficients binom(4,k-1) for k=1..5 */
        static const int B[5] = {1, 4, 6, 4, 1};

        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t sum = cache[i - 1];                     /* a[i-1]          */
            int kmax = (i < 5) ? (int)i : 5;              /* up to min(5,i)  */
            for (int k = 2; k <= kmax; ++k) {
                seq_t prod = 1;
                /* product of (i-1)*(i-2)*...*(i-(k-1)) */
                for (int t = 1; t < k; ++t)
                    prod *= (seq_t)(i - t);
                seq_t term = cache[i - k] * prod * B[k - 1];
                sum += term;
            }
            cache[i] = -sum;
        }
        computed_max = n;
    }
    return cache[n];
}
