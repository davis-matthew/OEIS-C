/*
 * A355291  Expansion of e.g.f. exp(exp(x)*(exp(x) + 1) - 2).
 * https://oeis.org/A355291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (1 + 2^k) * binomial(n-1,k-1) * a(n-k); a(0)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed terms */
    static seq_t *a_cache = NULL;
    static size_t a_used = 0;   /* number of terms actually computed (>=1) */
    static size_t a_cap  = 0;   /* allocated capacity */

    /* static cache for powers of two: pow2[k] = 2^k */
    static seq_t *pow2_cache = NULL;
    static size_t pow2_used = 0;
    static size_t pow2_cap  = 0;

    size_t need = (size_t)n + 1;          /* required array size for index n */

    /* grow array for a_cache if necessary */
    if (need > a_cap) {
        size_t new_cap = a_cap ? a_cap << 1 : 1;
        while (new_cap < need) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap  = new_cap;
    }

    /* grow array for pow2_cache if necessary */
    if (need > pow2_cap) {
        size_t new_cap = pow2_cap ? pow2_cap << 1 : 1;
        while (new_cap < need) new_cap <<= 1;
        pow2_cache = (seq_t *)realloc(pow2_cache, new_cap * sizeof(seq_t));
        pow2_cap  = new_cap;
    }

    /* initialise first elements on first call */
    if (a_used == 0) {
        a_cache[0] = 1;
        a_used = 1;
    }
    if (pow2_used == 0) {
        pow2_cache[0] = 1;
        pow2_used = 1;
    }

    /* compute powers of two up to n if they are missing */
    for (size_t i = pow2_used; i <= n; ++i) {
        pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;
    }
    pow2_used = n + 1;

    /* compute a[i] for all i from a_used up to n */
    for (size_t i = a_used; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;                     /* C(i-1,0) */
        for (size_t k = 1; k <= i; ++k) {
            seq_t term = a_cache[i - k];
            term *= binom;                    /* * C(i-1,k-1) */
            seq_t weight = pow2_cache[k] + (seq_t)1; /* 1 + 2^k */
            term *= weight;                   /* * (1 + 2^k) */
            sum += term;
            if (k < i) {                     /* update binom for next k */
                seq_t num = binom * (seq_t)(i - k);
                binom = num / (seq_t)k;      /* exact division */
            }
        }
        a_cache[i] = sum;
    }
    a_used = n + 1;
    return a_cache[n];
}
