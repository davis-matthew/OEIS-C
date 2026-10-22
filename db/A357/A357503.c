/*
 * A357503  a(n) is the hafnian of the 2n X 2n symmetric matrix whose element
 *          (i,j) equals abs(i-j).
 * https://oeis.org/A357503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=0}^{n-1} binomial(2*n-1, 2*k+1) * a(k) *
 *            a(n-k-1) with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t n, idx_t k); */

static seq_t *a_cache = NULL;   /* cache of computed values a(0)…a(n) */
static idx_t  a_cache_size = 0; /* number of cached entries */

/* Ensure that a_cache contains values a(0)…a(nmax) (inclusive).  */
static void ensure_cache(idx_t nmax)
{
    if (a_cache_size > nmax)   /* already large enough */
        return;

    idx_t new_size = nmax + 1;
    size_t new_size_t = (size_t)new_size;

    /* Reallocate the cache (NULL pointer is allowed for realloc). */
    a_cache = (seq_t *)realloc(a_cache, new_size_t * sizeof(seq_t));
    if (!a_cache)
        abort();               /* avoid failure shadowing the function */

    /* initialize new entries to zero (values will be overwritten) */
    for (idx_t i = a_cache_size; i < new_size; ++i)
        a_cache[i] = 0;

    /* initialise a(0) if the cache is newly created */
    if (a_cache_size == 0)
        a_cache[0] = 1;          /* a(0) = 1 */

    /* compute new terms.  Start from the first uncached index. */
    for (idx_t n = a_cache_size; n <= nmax; ++n) {
        if (n == 0) /* already set */
            continue;

        idx_t N = 2 * n - 1;          /* 2n-1 for the binomial argument */
        seq_t sum = 0;

        /* iterate over k = 0 … n-1.
           Keep the current odd binomial coefficient binom(N, i) with i = 2k+1   */
        idx_t i = 1;                     /* first odd index */
        seq_t binom_odd = binomial(N, 1); /* binomial(N,1) = N */

        for (idx_t k = 0; k < n; ++k) {
            seq_t term = a_cache[k] * a_cache[n - 1 - k];
            term *= binom_odd;
            sum += term;

            /* move to the next odd i (i+2) if there is one */
            if (i + 2 <= N) {
                /* from i to i+1 (even)    c1 = binom(N, i+1) = binom_odd * (N-i)/(i+1) */
                seq_t c1 = binom_odd * (N - i) / (i + 1);
                /* from i+1 to i+2 (odd)  c2 = binom(N, i+2) = c1 * (N-i-1)/(i+2)   */
                seq_t c2 = c1 * (N - i - 1) / (i + 2);
                binom_odd = c2;
                i += 2;
            }
        }

        a_cache[n] = n * sum;
    }

    a_cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    ensure_cache(n);
    return a_cache[n];
}
