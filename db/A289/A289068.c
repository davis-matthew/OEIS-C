/*
 * A289068  Recurrence a(n+2) = Sum_{k=0..n} binomial(n,k)*a(k)*a(n+1-k) with
 *          a(0)=1, a(1)=-2.
 * https://oeis.org/A289068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=-2 a(n+2)=∑_{k=0..n} binomial(n,k)*a(k)*a(n+1-k)
 *            (n≥0)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 15, i64 n <= 23, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* array of computed terms */
static size_t a_cache_allocated = 0;   /* allocated size of a_cache */
static idx_t  a_computed_max = 0;  /* highest n for which a_cache[n] is valid */

/* Ensure a_cache has space for indices 0..need (inclusive).
   Allocate a new larger buffer if necessary, copying existing values. */
static void ensure_a_cache(idx_t need)
{
    if (a_cache && a_cache_allocated > (size_t)need)
        return;

    size_t new_size = a_cache_allocated ? a_cache_allocated * 2 : 2;
    while (new_size <= (size_t)need)
        new_size <<= 1;

    seq_t *tmp;
#ifdef __cplusplus
    tmp = new seq_t[new_size];
#else
    tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
    if (!tmp)
        abort();                  /* Out of memory */

    /* copy old values */
    if (a_cache) {
        size_t i;
        for (i = 0; i < a_cache_allocated; ++i)
            tmp[i] = a_cache[i];
#ifdef __cplusplus
        delete[] a_cache;
#else
        free(a_cache);
#endif
    }

    a_cache = tmp;
    a_cache_allocated = new_size;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Base cases handled directly */
    if (n == 0)
        return 1;
    if (n == 1)
        return -2;

    /* Make sure the cache can hold index n */
    ensure_a_cache(n);

    /* Initialize cache if this is the first call */
    if (a_computed_max == 0) {
        a_cache[0] = 1;
        a_cache[1] = -2;
        a_computed_max = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t idx = a_computed_max + 1; idx <= n; ++idx) {
        idx_t m = idx - 2;                 /* n in the recurrence */
        seq_t sum = 0;
        seq_t bin = 1;                     /* binomial(m,0) */

        for (idx_t k = 0; k <= m; ++k) {
            seq_t prod = bin * a_cache[k] * a_cache[idx - 1 - k];
            sum += prod;

            /* update binomial coefficient to binomial(m,k+1) */
            if (k < m)
                bin = bin * (seq_t)(m - k) / (seq_t)(k + 1);
        }

        a_cache[idx] = sum;
    }

    a_computed_max = n;
    return a_cache[n];
}
