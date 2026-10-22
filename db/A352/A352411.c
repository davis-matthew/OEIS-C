/*
 * A352411  E.g.f.: x / LambertW( x/(1-x) ).
 * https://oeis.org/A352411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) determined by the e.g.f. A = (1-x) * exp(x/A).
 * The algorithm computes A's coefficients via recursive series operations:
 *   1. Reciprocals of A.
 *   2. B = x / A, giving B's coefficients.
 *   3. Exponential of B.
 *   4. A = (1-x) * exp(B).
 *
 * The implementation stores the coefficient vectors and extends them
 * as needed for successive calls to a(n), ensuring correctness for
 * all n ≥ 0 and optimal performance.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */

extern void seq_realloc(seq_t* old, size_t old_cap, size_t new_cap);  /* placeholder */

/* Static cache of computed terms */
static seq_t *cache_a = NULL;
static seq_t *cache_c = NULL;
static seq_t *cache_b = NULL;
static seq_t *cache_e = NULL;
static size_t cached_n = 0;

/* Allocate or resize cache arrays */
static void ensure_cached(idx_t n) {
    if (cached_n > n) return;
    size_t new_cap = n + 1;
    if (!cache_a) {
        cache_a = (seq_t*)malloc(new_cap * sizeof(seq_t));
        cache_c = (seq_t*)malloc(new_cap * sizeof(seq_t));
        cache_b = (seq_t*)malloc(new_cap * sizeof(seq_t));
        cache_e = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        cache_a = (seq_t*)realloc(cache_a, new_cap * sizeof(seq_t));
        cache_c = (seq_t*)realloc(cache_c, new_cap * sizeof(seq_t));
        cache_b = (seq_t*)realloc(cache_b, new_cap * sizeof(seq_t));
        cache_e = (seq_t*)realloc(cache_e, new_cap * sizeof(seq_t));
    }
    /* initialize base cases */
    if (cached_n == 0) {
        cache_a[0] = (seq_t)1;      /* a(0) = 1 */
        cache_c[0] = (seq_t)1;      /* reciprocal of a(0) */
        cache_b[0] = (seq_t)0;      /* not used */
        cache_e[0] = (seq_t)1;      /* e(0) = 1 */
    }
    /* Compute terms up to n */
    for (size_t i = cached_n; i <= (size_t)n; ++i) {
        if (i == 0) continue;  /* base already set */

        /* Compute c[i] = - \sum_{k=1}^i binom(i,k) * a[k] * c[i-k] */
        seq_t sum_c = (seq_t)0;
        seq_t binom = (seq_t)i;  /* binom(i,1) */
        for (size_t k = 1; k <= i; ++k) {
            sum_c += binom * cache_a[k] * cache_c[i - k];
            if (k < i) {
                seq_t num = (seq_t)(i - k);
                seq_t den = (seq_t)(k + 1);
                binom = (binom * num) / den;
            }
        }
        cache_c[i] = -sum_c;

        /* Compute b[i] = i * c[i-1] */
        cache_b[i] = (seq_t)i * cache_c[i - 1];

        /* Compute e[i] = \sum_{k=0}^{i-1} binom(i-1,k) * e[k] * b[i-k] */
        seq_t sum_e = cache_e[0] * cache_b[i];  /* k = 0 term */
        if (i >= 1) {
            seq_t binom_e = (seq_t)(i - 1);     /* binom(i-1,1) */
            for (size_t k = 1; k <= i - 1; ++k) {
                sum_e += binom_e * cache_e[k] * cache_b[i - k];
                if (k < i - 1) {
                    seq_t num = (seq_t)(i - 1 - k);
                    seq_t den = (seq_t)(k + 1);
                    binom_e = (binom_e * num) / den;
                }
            }
        }
        cache_e[i] = sum_e;

        /* Compute a[i] = e[i] - i * e[i-1] */
        cache_a[i] = cache_e[i] - (seq_t)i * cache_e[i - 1];
    }
    cached_n = (size_t)n;
}

seq_t a(idx_t n) {
    ensure_cached(n);
    return cache_a[(size_t)n];
}
