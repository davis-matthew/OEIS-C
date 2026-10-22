/*
 * A352292  Expansion of e.g.f. 1/(2 - exp(x) - x/(1 - x)).
 * https://oeis.org/A352292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k! + 1) * binomial(n,k) * a(n-k) with a(0)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values of a(n) and factorials */
    static seq_t *a_cache = NULL;     /* a[0..cap-1]   */
    static size_t a_cap   = 0;
    static seq_t *fact = NULL;        /* fact[0..cap-1] */
    static size_t fact_cap = 0;

    /* Grow cache to at least n+1 entries if needed */
    if (n >= a_cap) {
        size_t new_cap = n + 1;

        /* allocate or grow a_cache */
        if (a_cache == NULL) {
            a_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!a_cache) { /* allocation failure */
                return (seq_t)0;
            }
        } else if (a_cap < new_cap) {
            a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            if (!a_cache) {
                return (seq_t)0;
            }
        }
        a_cap = new_cap;

        /* allocate or grow factorial array */
        if (fact == NULL) {
            fact = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!fact) {
                return (seq_t)0;
            }
        } else if (fact_cap < new_cap) {
            fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
            if (!fact) {
                return (seq_t)0;
            }
        }
        fact_cap = new_cap;

        /* compute factorials up to n */
        fact[0] = (seq_t)1;
        for (idx_t k = 1; k <= n; ++k) {
            fact[k] = fact[k - 1] * (seq_t)k;
        }

        /* set a(0) = 1 if not already set */
        a_cache[0] = (seq_t)1;

        /* compute a(m) for m = 1 .. n */
        for (idx_t m = 1; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            seq_t comb = (seq_t)1; /* binom(m,0) */
            for (idx_t k = 1; k <= m; ++k) {
                /* update comb to binom(m,k) */
                comb = comb * (seq_t)(m - k + 1) / (seq_t)k;

                /* term = (k! + 1) * binom(m,k) * a(m-k) */
                seq_t term = (fact[k] + (seq_t)1) * comb;
                term *= a_cache[m - k];

                sum += term;
            }
            a_cache[m] = sum;
        }
    }

    return a_cache[n];
}
