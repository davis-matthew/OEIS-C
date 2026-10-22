/*
 * A384025  a(n) = [x^(2*n)] Product_{k=0..n} (1 + k*x)^3.
 * https://oeis.org/A384025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^(2*n) in ∏_{k=0}^{n} (1 + k⋅x)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state: highest n already processed, polynomial coefficients,
       and a cache of the a(k) values. */
    static idx_t cur_n = 0;          /* largest n already processed */
    static seq_t *coeff   = NULL;    /* polynomial coefficients */
    static size_t coeff_len = 0;     /* allocated length of coeff array */
    static seq_t *a_cache = NULL;    /* cache of a(k) values */
    static size_t a_cache_len = 0;   /* allocated length of a_cache */

    /* first call – initialise arrays and a(0) = 1 */
    if (coeff == NULL) {
        coeff = (seq_t *)malloc(sizeof(seq_t));
        if (!coeff) abort();
        coeff[0] = (seq_t)1;            /* product of the empty set is 1 */
        coeff_len = 1;
    }
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = coeff[0];
        a_cache_len = 1;
    }

    /* return cached value if already computed */
    if (n <= cur_n) return a_cache[n];

    /* extend computation up to the requested n */
    for (idx_t k = cur_n + 1; k <= n; ++k) {
        /* values for this step */
        seq_t k_seq = (seq_t)k;
        seq_t k2_seq = k_seq * k_seq;
        seq_t k3_seq = k2_seq * k_seq;
        seq_t f1 = k_seq * (seq_t)3;        /* 3*k */
        seq_t f2 = k2_seq * (seq_t)3;       /* 3*k^2 */

        /* old and new degree bounds */
        size_t old_deg = 3 * (size_t)(k - 1);
        size_t new_deg = 3 * (size_t)k;
        size_t new_len = new_deg + 1;

        /* enlarge coeff array if necessary, zeroing new slots */
        if (new_len > coeff_len) {
            seq_t *tmp = (seq_t *)realloc(coeff, new_len * sizeof(seq_t));
            if (!tmp) abort();
            if (new_len > coeff_len)
                memset(tmp + coeff_len, 0, (new_len - coeff_len) * sizeof(seq_t));
            coeff = tmp;
            coeff_len = new_len;
        }

        /* in‑place update from high to low */
        for (size_t d = new_deg; d > 0; --d) {
            seq_t val = coeff[d];          /* old[d] */

            seq_t sum = val;
            if (d >= 1) sum += coeff[d-1] * f1;
            if (d >= 2) sum += coeff[d-2] * f2;
            if (d >= 3) sum += coeff[d-3] * k3_seq;
            coeff[d] = sum;
        }
        /* coeff[0] stays 1 (constant term is unchanged) */

        /* store a(k) = coeff[2*k] */
        if (a_cache_len <= (size_t)k) {
            seq_t *tmp = (seq_t *)realloc(a_cache, ((size_t)k + 1) * sizeof(seq_t));
            if (!tmp) abort();
            a_cache = tmp;
            a_cache_len = (size_t)k + 1;
        }
        a_cache[k] = coeff[2 * (size_t)k];
    }

    /* now n is guaranteed to be <= cur_n and cached */
    cur_n = n;   /* update current state */
    return a_cache[n];
}
