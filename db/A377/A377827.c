/*
 * A377827  E.g.f. satisfies A(x) = (1 + x)^2 * exp(x * A(x)).
 * https://oeis.org/A377827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(k-1) * binomial(2*k+2,n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the exponential generating function */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for computed terms */
    static seq_t *a_cache = NULL;
    static seq_t *e_cache = NULL;
    static idx_t cache_size = 0;

    /* if term already computed, return it */
    if (n < cache_size)
        return a_cache[n];

    /* enlarge caches to hold terms up to n */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_a = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    seq_t *new_e = (seq_t *)realloc(e_cache, new_size * sizeof(seq_t));
    if (!new_a || !new_e) {
        /* allocation failed – abort the program */
        abort();
    }
    a_cache = new_a;
    e_cache = new_e;

    /* initialise the very first terms */
    if (cache_size == 0) {
        a_cache[0] = 1;
        e_cache[0] = 1;
    }

    /* compute new terms from cache_size up to n */
    for (idx_t m = cache_size; m <= n; ++m) {
        if (m == 0)                        /* first term already set */
            continue;

        /* compute e_m  (coefficients of exp(x*A(x))) */
        seq_t sum_e = 0;
        seq_t binom = 1;                    /* C(m-1,0) */
        for (idx_t t = 1; t <= m; ++t) {
            seq_t b_t   = (seq_t)t * a_cache[t - 1];    /* b_t = t * a_{t-1} */
            seq_t term  = binom * b_t;
            term        = term * e_cache[m - t];
            sum_e      += term;

            if (t < m) {                  /* update C(m-1,t) for next t */
                binom = binom * (seq_t)(m - t);
                binom = binom / (seq_t)t;
            }
        }
        e_cache[m] = sum_e;

        /* compute a_m  from e_m, e_{m-1}, e_{m-2} */
        seq_t val = e_cache[m];                                 /* k = 0 contribution */

        /* k = 1 contribution: 2 * m * e_{m-1} */
        val += (seq_t)m * (seq_t)2 * e_cache[m - 1];

        /* k = 2 contribution: 2 * C(m,2) * e_{m-2} */
        if (m >= 2) {
            seq_t binom_n2 = (seq_t)m * (seq_t)(m - 1) / (seq_t)2; /* C(m,2) */
            val += binom_n2 * (seq_t)2 * e_cache[m - 2];
        }

        a_cache[m] = val;
    }

    cache_size = n + 1;
    return a_cache[n];
}
