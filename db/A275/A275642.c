/*
 * A275642  Expansion of (1-q)^k/Product_{j=1..k} (1-q^j) for k=8.
 * https://oeis.org/A275642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in (1-q)^8 / Π_{j=1..8}(1-q^j)
 * Range:     i8 n <= 13, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Precomputed polynomial coefficients p[0..36] for P(q)=∏_{j=1}^8 (1-q^j) */
    static seq_t p[37];
    static bool p_initialized = false;
    if (!p_initialized) {
        for (int i = 0; i <= 36; ++i) p[i] = 0;
        p[0] = 1;
        int max_deg = 0;
        for (int t = 1; t <= 8; ++t) {
            for (int i = max_deg; i >= 0; --i) {
                p[i + t] -= p[i];
            }
            max_deg += t;           /* after multiplication degree increases by t */
        }
        p_initialized = true;
    }

    /* Coefficient q_n of Q(q)=(1-q)^8: nonzero only for 0≤n≤8 */
    static const seq_t q_coef[9] = {
        (seq_t)1,
        -(seq_t)8,
        (seq_t)28,
        -(seq_t)56,
        (seq_t)70,
        -(seq_t)56,
        (seq_t)28,
        -(seq_t)8,
        (seq_t)1
    };

    /* Dynamic cache of already computed a(k) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of entries currently stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Grow cache to at least n+1 entries */
    if (cache_size <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
        /* Compute new terms */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            size_t up = (i <= 36) ? i : 36;
            for (size_t j = 1; j <= up; ++j) {
                sum += p[j] * cache[i - j];
            }
            seq_t q = (i <= 8) ? q_coef[i] : 0;
            cache[i] = q - sum;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
