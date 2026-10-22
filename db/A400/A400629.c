/*
 * A400629  a(1) = 1; a(n+1) = Sum_{d|n} binomial(n,d) * a(n/d) * a(d).
 * https://oeis.org/A400629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(1) = 1; a(n+1) = Σ_{d|n} binomial(n,d) * a(n/d) * a(d) for n ≥
 *            1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file relies only on the standard headers and the types
   seq_t and idx_t provided by the build environment. */

#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Multiplicative binomial coefficient: binom(n,k) = C(n,k)           */
/* It uses only +, -, *, / and works with any seq_t that supports    */
/* these operators.  The recursion is safe for all n that fit the    */
/* sequence result in the given seq_t type.                           */
/* ------------------------------------------------------------------ */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* r = r * (n - k + i) / i   – division is exact each step  */
        r = r * (n - k + i) / i;
    }
    return r;
}

/* ------------------------------------------------------------------ */
/* Cache of previously computed values.  The array is 1‑based; index 0 */
/* is unused.  Cache grows as needed.                                 */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;   /* 1‑based array of a(i) values */
static size_t cache_cap  = 0;       /* allocated capacity (elements)   */
static idx_t   cached_n   = 0;      /* largest index computed           */

/* ------------------------------------------------------------------ */
/* Ensure that a(1) … a(n) are present in the cache.  Computes any   */
/* missing terms in ascending order, which is valid regardless of    */
/* the order in which a() is called.                                 */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow the cache array if necessary */
    if (!cache || (size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        /* In C++ realloc returns void*, cast to seq_t* is required */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the base case a(1) the first time */
    if (cached_n == 0) {
        cache[1] = (seq_t)1;
        cached_n = 1;
    }

    /* Compute any missing values up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        idx_t m   = i - 1;                 /* the n in the recurrence */
        seq_t sum = (seq_t)0;

        /* Enumerate all divisors d of m */
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t d2   = m / d;
                seq_t term1 = binom(m, d) * cache[d] * cache[d2];
                sum += term1;

                if (d2 != d) {
                    seq_t term2 = binom(m, d2) * cache[d2] * cache[d];
                    sum += term2;
                }
            }
        }
        cache[i] = sum;
    }

    cached_n = n;
}

/* ------------------------------------------------------------------ */
/* External function required by the problem description.             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) abort();           /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
