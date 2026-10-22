/*
 * A382406  Expansion of 1/(1 - x*(1 + x)^2)^3.
 * https://oeis.org/A382406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+2,2) * binomial(2*k,n-k)
 *            Recurrence: a(n) =
 *            3*a(n-1)+3*a(n-2)-8*a(n-3)-12*a(n-4)+3*a(n-5)+17*a(n-6)+15*a(n-7)+6*a(n-8)+a(n-9)
 *            The first ten terms are generated from the summation; the
 *            recurrence is used for n ≥ 10. Memoisation with a growing static
 *            array guarantees...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 106,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

static seq_t *cache = NULL;       /* memoised values */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of valid entries */

/* ------------------------------------------------------------------ */
/* Compute a(n) by the defining sum.  n is always < 10 in practice for
   this routine, but the code works for any n that fits in a 64‑bit int. */
static seq_t sum_formula(idx_t n) {
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c1 = binomial(k + 2, 2);           /* (k+2 choose 2) */
        seq_t c2 = binomial(2 * k, n - k);       /* (2k choose n-k) */
        total += c1 * c2;
    }
    return total;
}

/* ------------------------------------------------------------------ */
/* Ensure the memoisation table exists and contains the first ten
   terms produced by the sum formula. */
static void init_cache(void) {
    if (cache_len > 0)
        return;                     /* already initialised */

    cache_cap = 10;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);

    for (idx_t i = 0; i < 10; ++i)
        cache[i] = sum_formula(i);

    cache_len = 10;
}

/* ------------------------------------------------------------------ */
/* Ensure the cache has capacity for values up to index n (inclusive). */
static void ensure_capacity(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);

    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main public function */
seq_t a(idx_t n) {
    init_cache();

    if (n < cache_len)
        return cache[n];

    ensure_capacity(n);

    /* Compute new terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t value;
        if (i < 10) {
            value = sum_formula(i);
        } else {
            /* Separate positive and negative parts to avoid underflow
               on unsigned types – the recurrence guarantees positivity. */
            seq_t pos = (cache[i - 1] * (seq_t)3) +
                        (cache[i - 2] * (seq_t)3) +
                        (cache[i - 5] * (seq_t)3) +
                        (cache[i - 6] * (seq_t)17) +
                        (cache[i - 7] * (seq_t)15) +
                        (cache[i - 8] * (seq_t)6) +
                        (cache[i - 9]);

            seq_t neg = (cache[i - 3] * (seq_t)8) +
                        (cache[i - 4] * (seq_t)12);

            value = pos - neg;
        }
        cache[i] = value;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
