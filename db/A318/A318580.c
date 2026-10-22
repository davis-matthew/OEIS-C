/*
 * A318580  Expansion of e.g.f. exp(-1 + Product_{k>=1} 1/(1 - x^k)^k).
 * https://oeis.org/A318580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n)=sum_{k=1..n}
 *            A000219(k)*k!*C(n-1,k-1)*a(n-k), where A000219(k) is the
 *            coefficient of x^k in ∏_{j>=1} 1/(1-x^j)^j. A000219 can be
 *            computed from the recurrence c_0 = 1 c_n = (1/n) * Σ_{k=1..n}
 *            σ₂(k) * c_{n-k}, with σ₂(k)=sum of squares of divisors of k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised arrays. */
static seq_t *a_cache   = NULL;   /* a values */
static seq_t *fact_cache = NULL;  /* k! */
static seq_t *c_cache   = NULL;   /* A000219 values */
static seq_t *sigma_cache = NULL; /* σ₂ values */

static idx_t cached_max = 0; /* highest index computed */

seq_t a(idx_t n)
{
    /* First call: allocate and initialise for n=0 */
    if (cached_max == 0 && a_cache == NULL) {
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        fact_cache = (seq_t*)malloc(sizeof(seq_t));
        c_cache = (seq_t*)malloc(sizeof(seq_t));
        sigma_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!a_cache || !fact_cache || !c_cache || !sigma_cache) return (seq_t)0;
        a_cache[0]   = (seq_t)1;
        fact_cache[0] = (seq_t)1;
        c_cache[0]   = (seq_t)1;
        /* sigma_cache[0] unused, leave zero */
        cached_max = 0;
    }

    /* If requested n already known, return */
    if (n <= cached_max) return a_cache[n];

    /* Grow arrays to size n+1 */
    size_t new_size = (size_t)n + 1;
    a_cache = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
    fact_cache = (seq_t*)realloc(fact_cache, new_size * sizeof(seq_t));
    c_cache = (seq_t*)realloc(c_cache, new_size * sizeof(seq_t));
    sigma_cache = (seq_t*)realloc(sigma_cache, new_size * sizeof(seq_t));
    if (!a_cache || !fact_cache || !c_cache || !sigma_cache) return (seq_t)0;

    /* Zero new entries */
    if (new_size > cached_max + 1) {
        size_t old = (size_t)cached_max + 1;
        size_t diff = new_size - old;
        memset(a_cache + old,   0, diff * sizeof(seq_t));
        memset(fact_cache + old,0, diff * sizeof(seq_t));
        memset(c_cache + old,   0, diff * sizeof(seq_t));
        memset(sigma_cache + old,0, diff * sizeof(seq_t));
    }

    /* Recompute σ₂ values up to n */
    for (idx_t i = 1; i <= n; ++i) sigma_cache[i] = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t sq = ii * ii;
        for (idx_t j = i; j <= n; j += i) {
            sigma_cache[j] = sigma_cache[j] + sq;
        }
    }

    /* Compute factorials from cached_max+1 to n */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        fact_cache[k] = fact_cache[k - 1] * (seq_t)k;
    }

    /* Compute c (A000219) values from cached_max+1 to n */
    for (idx_t m = cached_max + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            sum = sum + sigma_cache[k] * c_cache[m - k];
        }
        c_cache[m] = sum / (seq_t)m; /* exact division */
    }

    /* Compute a values from cached_max+1 to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1; /* binom(i-1,0) */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = c_cache[k];
            term = term * fact_cache[k];
            term = term * bin;
            term = term * a_cache[i - k];
            sum = sum + term;
            if (k < i) {
                /* bin = bin * (i - k) / k */
                seq_t mult = (seq_t)(i - k);
                bin = bin * mult;
                bin = bin / (seq_t)k;
            }
        }
        a_cache[i] = sum;
    }

    cached_max = n;
    return a_cache[n];
}
