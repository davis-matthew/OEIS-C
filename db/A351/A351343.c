/*
 * A351343  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 + x^4 * A(x/(1 -
 *          2*x)) / (1 - 2*x).
 * https://oeis.org/A351343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A351343
 *
 *  Recurrence (from the OEIS entry):
 *      a(0) = a(1) = a(2) = a(3) = 1
 *      a(n) = Σ_{k=0}^{n-4} binomial(n-4, k) * 2^k * a(n-4-k)  for n ≥ 4
 *
 *  The implementation memoises computed terms in a dynamic cache so that
 *  calls with arbitrary n, in any order, are fast.  It works for any
 *  seq_t that supports the usual arithmetic operators and for any
 *  idx_t that is an unsigned integer type (e.g. uint64_t).
 */

#include <stdlib.h>          /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;   /* pointer to cache array */
    static size_t cache_cap  = 0;      /* current capacity of cache */
    static idx_t last_computed = 0;    /* highest index that has been calculated */
    static int initialized = 0;       /* have we stored the base values? */

    /* trivial case: first four terms are 1 */
    if (n < 4)
        return (seq_t)1;

    /* make sure the cache can hold indices 0..n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        /* cast required for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                   /* allocation failed – return 0 */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the base terms once */
    if (!initialized) {
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = (seq_t)1;
        last_computed = 3;
        initialized   = 1;
    }

    /* compute any missing terms up to n (or just n if the cache is already up‑to‑date) */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* terms 0..3 are already 1 – the loop only reaches i ≥ 4 */
        idx_t m    = i - 4;           /* number of summation terms */
        seq_t comb = (seq_t)1;        /* binomial coefficient C(m,0) */
        seq_t twos = (seq_t)1;        /* 2^0 */
        seq_t sum  = (seq_t)0;

        for (idx_t k = 0; k <= m; ++k) {
            /* term = C(m,k) * 2^k * a(m-k)  (a(m-k) = cache[i-4-k]) */
            seq_t term = comb * twos;
            term = term * cache[i - 4 - k];
            sum += term;

            /* update binomial coefficient and power of two for next k */
            if (k != m) {
                twos = twos * (seq_t)2;
                /* comb = comb * (m - k) / (k + 1);   division is exact */
                comb = (comb * (seq_t)(m - k)) / (seq_t)(k + 1);
            }
        }
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
