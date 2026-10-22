/*
 * A377966  Expansion of e.g.f. (1+x)^2 * exp(x*(1+x)^3).
 * https://oeis.org/A377966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Σ_{k=0..n} binom(3*k+2, n-k) / k! Recurrence (from
 *            the OEIS entry) gives a(n+5) from a(n)...a(n+4): a(n+5) =
 *            4*(n+1)(n+2)(n+3)(n+4)*a(n) + 13*(n+2)(n+3)(n+4)*a(n+1) +
 *            15*(n+4)(n+3)*a(n+2) + 7*(n+4)*a(n+3) – (n+1)*a(n+4) Initial
 *            values: a(0)=1, a(1)=3, a(2)=13, a(3)=85, a(4)=621, a(5)=...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (in seq_t elements) */
    static size_t cache_size = 0;  /* number of terms actually stored   */

    /* initial values, supplied in order of n = 0..5 */
    static const seq_t init_vals[6] = {
        (seq_t)1, (seq_t)3, (seq_t)13,
        (seq_t)85, (seq_t)621, (seq_t)5131
    };

    /* ensure we have storage for at least n+1 elements */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory */
        cache = tmp;
        /* zero initialise the newly allocated part (not strictly needed) */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* if the term is already computed, return it immediately */
    if (n < cache_size) return cache[(size_t)n];

    /* initialise the first six terms once, if necessary */
    if (cache_size == 0) {
        if (cache_cap < 6) {
            size_t new_cap = 6;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = 0; i < 6; ++i) cache[i] = init_vals[i];
        cache_size = 6;
    }

    /* compute terms from cache_size up to n (inclusive) */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i < 6) {
            /* values 0..5 are already in cache */
            continue;
        }
        /* Compute a(i) using the recurrence with k = i-5. */
        idx_t k = i - 5;

        /* term1 = 4*(k+1)(k+2)(k+3)(k+4)*a(k) */
        seq_t term1 = cache[k];
        term1 *= (k + 1);
        term1 *= (k + 2);
        term1 *= (k + 3);
        term1 *= (k + 4);
        term1 *= 4;

        /* term2 = 13*(k+2)(k+3)(k+4)*a(k+1) */
        seq_t term2 = cache[k + 1];
        term2 *= (k + 2);
        term2 *= (k + 3);
        term2 *= (k + 4);
        term2 *= 13;

        /* term3 = 15*(k+4)(k+3)*a(k+2) */
        seq_t term3 = cache[k + 2];
        term3 *= (k + 4);
        term3 *= (k + 3);
        term3 *= 15;

        /* term4 = 7*(k+4)*a(k+3) */
        seq_t term4 = cache[k + 3];
        term4 *= (k + 4);
        term4 *= 7;

        /* term5 = -(k+1)*a(k+4) */
        seq_t term5 = cache[k + 4];
        term5 *= (k + 1);
        /* subtraction is handled by adding the negative below */

        seq_t result = term1;
        result += term2;
        result += term3;
        result += term4;
        result -= term5;

        cache[i] = result;
    }

    cache_size = n + 1;
    return cache[(size_t)n];
}
