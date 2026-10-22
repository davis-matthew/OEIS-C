/*
 * A380762  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x * (1 +
 *          x)^2) / (1 + x) ).
 * https://oeis.org/A380762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+1)^(k-1) * binomial(n+2*k+1, n-k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Cache for factorials */
static seq_t *fact = NULL;
static size_t fact_cap = 0;

/* Cache for already computed a(n) values (0 means not yet computed) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that factorials and cache are allocated up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    /* Expand factorial cache if needed */
    if ((size_t)n >= fact_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = fact ? (seq_t *)realloc(fact, new_cap * sizeof(seq_t))
                          : (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();
        fact = tmp;
        /* Compute missing factorials */
        for (size_t i = fact_cap; i < new_cap; ++i) {
            if (i == 0)
                fact[i] = (seq_t)1;
            else
                fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }

    /* Expand a(n) cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = cache ? (seq_t *)realloc(cache, new_cap * sizeof(seq_t))
                          : (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* Zero‑initialize new slots */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)        /* a(0) = 1 */
        return (seq_t)1;

    ensure_capacity(n);

    /* Return cached value if present */
    if (cache[n] != (seq_t)0)
        return cache[n];

    seq_t fact_n = fact[n];
    seq_t sum = fact_n;            /* term for k = 0 */

    /* Binomial coefficient B(k) = C(n+2*k+1, n-k) */
    seq_t binom = (seq_t)(n + 1);   /* B(0) */

    seq_t ratio = fact_n;          /* n! / k! for current k (starts with k=0) */
    seq_t pow   = (seq_t)1;        /* (n+1)^(k-1) for k = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binom from B(k-1) to B(k) using the recurrence */
        idx_t prev_k = k - 1;
        seq_t num = binom;
        num *= (seq_t)(n + 2 * prev_k + 3); /* n+2k+1 */
        num *= (seq_t)(n + 2 * prev_k + 2); /* n+2k   */
        num *= (seq_t)(n - prev_k);         /* n-prev_k*/
        seq_t denom = (seq_t)(3 * prev_k + 2);
        denom *= (seq_t)(3 * prev_k + 3);
        denom *= (seq_t)(3 * prev_k + 4);
        binom = num / denom;               /* exact division */

        if (k > 1)           /* ratio = n! / k!  (for k ≥ 2) */
            ratio /= (seq_t)k;

        /* term = ratio * binom * pow */
        seq_t term = ratio;
        term *= binom;
        term *= pow;
        sum += term;

        /* Prepare pow for next k (k+1) */
        pow *= (seq_t)(n + 1);
    }

    cache[n] = sum;
    return sum;
}
