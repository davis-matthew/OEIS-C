/*
 * A351028  G.f. A(x) satisfies: A(x) = x + x^2 * A(x/(1 - 2*x)) / (1 - 2*x).
 * https://oeis.org/A351028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ∑_{k=0..n-2} binomial(n-2,k) * 2^k * a(n-k-2)
 * with a(0) = 0, a(1) = 1.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* static cache for already computed values */
static seq_t  *cache   = NULL;   /* array of a(0)..a(n) */
static bool   *computed = NULL;  /* same size, true if value is filled */
static size_t  cache_cap = 0;     /* current allocation size */

/* enlarge the cache so that it can hold values up to index n */
static void ensure_cache(idx_t n)
{
    size_t sz = (size_t)n + 1;      /* we need indices 0 .. n inclusive */
    if (cache_cap >= sz)
        return;

    seq_t  *new_cache   = (seq_t *)realloc(cache,   sz * sizeof(seq_t));
    bool   *new_comp    = (bool   *)realloc(computed, sz * sizeof(bool));
    if (!new_cache || !new_comp) {
        /* in a contest setting we exit on memory failure */
        exit(1);
    }

    /* initialize the newly added slots as uncomputed */
    for (size_t i = cache_cap; i < sz; ++i)
        new_comp[i] = false;

    cache = new_cache;
    computed = new_comp;
    cache_cap = sz;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* make sure the cache can hold up to index n */
    ensure_cache(n);

    /* initialise the first two entries once */
    if (!computed[0]) { cache[0] = 0; computed[0] = true; }
    if (!computed[1]) { cache[1] = 1; computed[1] = true; }

    size_t n_s = (size_t)n;

    /* compute any missing values up to n */
    for (size_t i = 2; i <= n_s; ++i) {
        if (computed[i]) continue;

        size_t m = i - 2;          /* corresponds to n-2 in the formula */
        seq_t sum   = 0;
        seq_t binom = 1;           /* binomial(m,0) */
        seq_t pow2  = 1;           /* 2^0 */

        for (size_t k = 0; k <= m; ++k) {
            /* accumulate the term binom * 2^k * a(m-k) */
            sum += binom * pow2 * cache[m - k];

            if (k < m) {
                /* next binomial coefficient: C(m,k+1) = C(m,k) * (m-k)/(k+1) */
                binom = (binom * (seq_t)(m - k)) / (seq_t)(k + 1);
                /* next power of two: 2^(k+1) = 2^k * 2 */
                pow2 = pow2 * (seq_t)2;
            }
        }

        cache[i] = sum;
        computed[i] = true;
    }

    return cache[n_s];
}
