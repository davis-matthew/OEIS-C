/*
 * A302699  G.f. A(x) satisfies: A(x) = 1 + x * A(x) * (A(x) + 3*x*A'(x)) /
 *          (A(x) + x*A'(x)).
 * https://oeis.org/A302699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302699
 *
 * Derivation from the functional equation
 *   A(x) = 1 + x*A(x)*(A(x)+3*x*A'(x))/(A(x)+x*A'(x))
 *
 * Gives the following recurrence with a(0)=1:
 *
 *   a(n) = (3*n-2)*a(n-1)                                          \\
 *        + Σ_{i=1}^{n-1} a(i) * (3*n-3*i-2) * a(n-1-i)            \\
 *        - Σ_{i=1}^{n-1} (i+1) * a(i) * a(n-i)     ,   n ≥ 1
 *
 * All indices are 0‑based, so a(0) is the first term listed in OEIS.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached terms */
static idx_t cache_cap = 0;   /* current capacity of cache */
static idx_t cache_len = 0;   /* number of cached terms */

/* Ensure the cache can hold indices up to *needed* (inclusive) */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap <= needed) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= needed)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;  /* base case a(0)=1 */

    /* Make sure we have space for all requested indices */
    ensure_capacity(n);

    /* Initialise cache on first use */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        /* term = (3*k-2)*a(k-1)  */
        seq_t term = (seq_t)(3 * k - 2) * cache[k - 1];

        /* add Σ_{i=1}^{k-1} a(i) * (3*k-3*i-2) * a(k-1-i) */
        for (idx_t i = 1; i < k; ++i) {
            seq_t coef = (seq_t)(3 * k - 3 * i - 2);
            term += cache[i] * coef * cache[k - 1 - i];
        }

        /* subtract Σ_{i=1}^{k-1} (i+1) * a(i) * a(k-i) */
        for (idx_t i = 1; i < k; ++i) {
            term -= (seq_t)(i + 1) * cache[i] * cache[k - i];
        }

        cache[k] = term;
    }

    cache_len = n + 1;  /* all terms up to a(n) are now cached */

    return cache[n];
}
