/*
 * A323500  Number of minimum dominating sets in the n X n black bishop graph.
 * https://oeis.org/A323500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n/2)! * (n+1)/2 if n mod 4 = 0 a(n) = ((n-1)/2)! * (n^3
 *            + 3*n^2 + 2*n - 2)/8 if n mod 4 = 1, n > 1 a(n) = (n/2-1)! *
 *            (n^2 + n + 2)/4 if n mod 4 = 2 a(n) = ((n-1)/2)! if n mod 4 = 3
 *            a(1) = 1 (All operations are performed on type seq_t, converting
 *            n first.)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 36, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached factorials: fact_cache[i] = i!  (i = 0 .. fact_cache_size-1) */
static seq_t *fact_cache = NULL;
static size_t  fact_cache_size = 0;   /* number of cached factorials */

/* Ensure that factorials up to m (inclusive) are available in fact_cache. */
static void ensure_factorial(idx_t m)
{
    /* Need at least m+1 elements. If we already have it, nothing to do. */
    if ((size_t)m + 1 <= fact_cache_size)
        return;

    size_t new_size = (size_t)m + 1;
    seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();   /* out of memory */
    fact_cache = tmp;

    /* If the cache was empty before, initialise 0! */
    if (fact_cache_size == 0) {
        fact_cache[0] = (seq_t)1;   /* 0! = 1 */
    }

    /* Compute new factorials from the previous size up to m. */
    size_t start = fact_cache_size;          /* existing number of factorials */
    if (start == 0)                         /* we just initialised 0! */
        start = 1;                          /* start after 0! */

    for (size_t i = start; i <= (size_t)m; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }

    fact_cache_size = new_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t r = n % 4;
    seq_t nseq = (seq_t)n;

    if (r == 0) {                      /* n mod 4 == 0 */
        idx_t m = n / 2;
        ensure_factorial(m);
        seq_t fac = fact_cache[m];
        return (fac * (nseq + (seq_t)1)) / (seq_t)2;
    }

    if (r == 1) {                      /* n mod 4 == 1, n > 1 */
        idx_t m = (n - (idx_t)1) / 2;
        ensure_factorial(m);
        seq_t fac = fact_cache[m];

        seq_t n2   = nseq * nseq;                  /* n^2 */
        seq_t n3   = n2 * nseq;                   /* n^3 */
        seq_t numer = n3 + 3 * n2 + 2 * nseq - (seq_t)2;
        return (fac * numer) / (seq_t)8;
    }

    if (r == 2) {                      /* n mod 4 == 2 */
        idx_t m = n / 2 - (idx_t)1;
        ensure_factorial(m);
        seq_t fac = fact_cache[m];

        seq_t numer = nseq * nseq + nseq + (seq_t)2;   /* n^2 + n + 2 */
        return (fac * numer) / (seq_t)4;
    }

    /* r == 3 : n mod 4 == 3 */
    idx_t m = (n - (idx_t)1) / 2;
    ensure_factorial(m);
    return fact_cache[m];
}
