/*
 * A318108  a(n) = Sum_{k=0..n} (3*n-2*k)!/((n-k)!^3*k!)*(-3)^k, n >= 0.
 * https://oeis.org/A318108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-3)^(n-k) * binomial(n,k) *
 *            binomial(n+2*k,n) * binomial(2*k,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   compute_powers_of_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are provided by the build. */

#include <stdlib.h>
#include "../utils/compute_powers_of_three.h"

/* Main sequence function with static cache. */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len  = 0;   /* number of values stored (highest index + 1) */

    /* Allocate / enlarge cache if needed */
    if ((idx_t)cache_len == 0) { /* first call */
        cache_cap = 8;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Return cached value if already computed */
    if (n < cache_len) return cache[n];

    /* Ensure powers of 3 up to n are available */
    static seq_t *pow3 = NULL;
    static size_t pow3_cap = 0;
    if ((size_t)n >= pow3_cap) {
        size_t new_cap = pow3_cap ? pow3_cap * 2 : 8;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        pow3 = tmp;
        pow3_cap = new_cap;
        compute_powers_of_three((idx_t)(new_cap - 1), pow3);
    }
    /* Seed initial small values */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = 3;
        cache_len = 2;
    }
    if (cache_len == 2) {
        cache[2] = 27;
        cache_len = 3;
    }

    /* Compute a(i) for i from current cache_len to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Compute a(i) via the summation formula */
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* Initialize multiplicative sequences */
        seq_t binom_nk = 1;                      /* C(i,0) = 1 */
        seq_t c2k_k   = 1;                      /* C(0,0) = 1 */
        seq_t cn2k_n  = 1;                      /* C(i, i) = 1 */

        for (idx_t k = 0; k <= i; ++k) {
            /* Term = binom_nk * pow3[i-k] * cn2k_n * c2k_k */
            seq_t term = binom_nk * pow3[i - k];
            term = term * cn2k_n;
            term = term * c2k_k;

            /* Add to positive or negative sum based on parity of (i-k) */
            if (((i - k) & 1ULL) == 0)
                sum_pos = sum_pos + term;
            else
                sum_neg = sum_neg + term;

            /* Update sequences for next k */
            if (k == i) break;  /* avoid division by zero on last iteration */

            /* Update binomial C(i, k+1) */
            seq_t nk1_num  = (seq_t)(i - k);
            seq_t nk1_denom = (seq_t)(k + 1);
            binom_nk = (binom_nk * nk1_num) / nk1_denom;

            /* Update C(2k, k) -> C(2(k+1), k+1) */
            seq_t num2k2 = (seq_t)(2 * (k + 1));
            seq_t num2k1 = (seq_t)(2 * (k + 1) - 1);
            seq_t denom2k = (seq_t)(k + 1);
            c2k_k = (c2k_k * num2k2 * num2k1) / (denom2k * denom2k);

            /* Update C(i+2k, i) -> C(i+2(k+1), i) */
            seq_t numn2k2 = (seq_t)(i + 2 * (k + 1));
            seq_t numn2k1 = (seq_t)(i + 2 * (k + 1) - 1);
            seq_t denomn2k = (seq_t)(2 * (k + 1));
            seq_t denomb1   = (seq_t)(2 * (k + 1) - 1);
            cn2k_n = (cn2k_n * numn2k2 * numn2k1) / (denomn2k * denomb1);
        }

        /* Final value a(i) = sum_pos - sum_neg (always non‑negative) */
        cache[i] = sum_pos - sum_neg;
    }

    cache_len = n + 1;
    return cache[n];
}
