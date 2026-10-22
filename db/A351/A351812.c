/*
 * A351812  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - 6*x)) / (1 - 6*x)^2.
 * https://oeis.org/A351812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=∑_{k=1..n} binomial(n,k-1) * 6^(k-1) *
 *            a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cap   = 0;            /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* Ensure a(0) exists */
    if (cap == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cap = 1;
        cache[0] = (seq_t)1;
        return cache[0];
    }

    /* If n is already computed, return it */
    if ((size_t)n < cap)
        return cache[n];

    /* Grow cache to size n+1 if necessary */
    size_t old_cap = cap;
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cap) {
        /* Use a C‑style cast so that the code compiles in C++ as well */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        /* Initialise the new part to zero (not strictly required) */
        for (size_t i = old_cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* Compute all missing values from old_cap up to n */
    for (idx_t m = (idx_t)old_cap; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;     /* binomial(m,0) */
        seq_t six_pow = (seq_t)1;   /* 6^0 */

        for (idx_t j = 0; j < m; ++j) {
            /* term: binom(m,j) * 6^j * a(m-1-j) */
            sum += binom * six_pow * cache[m - 1 - j];

            if (j + 1 < m) {
                /* Update binomial coefficient for next j */
                binom = binom * (seq_t)(m - j) / (seq_t)(j + 1);
                six_pow = six_pow * (seq_t)6;
            }
        }
        cache[m] = sum;
    }

    return cache[n];
}
