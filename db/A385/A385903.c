/*
 * A385903  Expansion of e.g.f. log((1-x)^4 / (x^4 - 5*x^3 + 6*x^2 - 4*x +
 *          1)).
 * https://oeis.org/A385903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A385903
 *  Recurrence (order 5):
 *      a(n+5) = (n‑5)*a(n)*(n‑4)*(n‑3)*(n‑2)*(n‑1)
 *            - 6*(n‑4)*(n‑3)*(n‑2)*(n‑1)*a(n+1)
 *            + 11*(n‑3)*(n‑2)*(n‑1)*a(n+2)
 *            - 10*(n‑2)*(n‑1)*a(n+3)
 *            + 5*(n‑1)*a(n+4)
 *  Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=6, a(4)=96
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation of computed values */
static seq_t *cache = NULL;      /* array of cached terms             */
static idx_t cache_cap = 0;      /* allocated capacity (in entries)   */
static idx_t max_computed = 0;   /* next index that has not been computed */

/* Ensure the cache holds at least 'needed' entries */
static void ensure_cache_size(idx_t needed)
{
    if (needed > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)needed * sizeof(seq_t));
        if (!new_cache) abort();          /* critical failure – terminate */
        cache = new_cache;
        cache_cap = needed;
    }
}

/* Compute a(n) for the OEIS sequence A385903 */
seq_t a(idx_t n)
{
    /* Initialise the first five terms (if not already done) */
    ensure_cache_size(5);
    if (max_computed < 5) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)96;
        max_computed = 5;
    }

    /* Return early if the value has already been computed */
    if (n < max_computed)
        return cache[(size_t)n];

    /* Ensure the cache can hold the requested index */
    ensure_cache_size((idx_t)n + 1);

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = max_computed; i <= n; ++i) {
        /* The recurrence applies only for i >= 5 */
        /* Compute a(i) from a(i‑5)…a(i‑1) */
        seq_t a0 = cache[(size_t)(i - 5)];
        seq_t a1 = cache[(size_t)(i - 4)];
        seq_t a2 = cache[(size_t)(i - 3)];
        seq_t a3 = cache[(size_t)(i - 2)];
        seq_t a4 = cache[(size_t)(i - 1)];

        seq_t term1 = a0;
        term1 *= (seq_t)(i - 5);
        term1 *= (seq_t)(i - 4);
        term1 *= (seq_t)(i - 3);
        term1 *= (seq_t)(i - 2);
        term1 *= (seq_t)(i - 1);

        seq_t term2 = a1;
        term2 *= (seq_t)6;
        term2 *= (seq_t)(i - 4);
        term2 *= (seq_t)(i - 3);
        term2 *= (seq_t)(i - 2);
        term2 *= (seq_t)(i - 1);

        seq_t term3 = a2;
        term3 *= (seq_t)11;
        term3 *= (seq_t)(i - 3);
        term3 *= (seq_t)(i - 2);
        term3 *= (seq_t)(i - 1);

        seq_t term4 = a3;
        term4 *= (seq_t)10;
        term4 *= (seq_t)(i - 2);
        term4 *= (seq_t)(i - 1);

        seq_t term5 = a4;
        term5 *= (seq_t)5;
        term5 *= (seq_t)(i - 1);

        cache[(size_t)i] = term1 - term2 + term3 - term4 + term5;
    }

    max_computed = n + 1;
    return cache[(size_t)n];
}
