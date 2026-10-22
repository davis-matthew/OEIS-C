/*
 * A360898  G.f. satisfies A(x) = 1 + x/(1 + x^3) * A(x/(1 + x^3)).
 * https://oeis.org/A360898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 29, i64 n <= 51, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360898:  a(0)=1; a(n)=Σ_{k=0..⌊(n‑1)/3⌋} (-1)^k · binom(n‑1‑2k,k) · a(n‑1‑3k)  */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Multiplicative binomial coefficient using seq_t arithmetic. */
static seq_t binomial_multiplicative(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;           /* use symmetry */
    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        result = result * (n - k + i);
        result = result / i;            /* exact division */
    }
    return result;
}

/* Memoisation cache for a(n). */
static seq_t *cache = NULL;            /* cache[i] holds a(i)           */
static size_t cache_allocated = 0;     /* number of entries allocated  */
static idx_t  cache_max_computed = (idx_t)-1; /* highest i already computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_allocated > (size_t)n) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                  /* unrecoverable */
    cache = tmp;
    cache_allocated = new_cap;
}

/* Compute a(i) for all i ≤ n, building on previously computed values. */
static void compute_up_to(idx_t n)
{
    /* First call: initialise cache[0] = 1. */
    if (cache_max_computed == (idx_t)-1)
    {
        ensure_cache(n);
        cache[0] = (seq_t)1;
        cache_max_computed = 0;
        if (n == 0) return;
    }
    if (cache_max_computed >= n) return;   /* already done */

    ensure_cache(n);

    for (idx_t i = cache_max_computed + 1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        idx_t limit = i - 1;
        for (idx_t k = (idx_t)0; 3 * k <= limit; ++k)
        {
            idx_t idx = limit - 3 * k;              /* n‑1‑3k   */
            seq_t bin = binomial_multiplicative(limit - 2 * k, k);
            seq_t sub = cache[idx];
            seq_t term = bin * sub;
            if (k & (idx_t)1)
                sum -= term;                         /* (-1)^k = –1 */
            else
                sum += term;                         /* (+1)     */
        }
        cache[i] = sum;
    }
    cache_max_computed = n;
}

/* Public interface required by the test harness. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
