/*
 * A336606  Sum_{n>=0} a(n) * x^n / (n!)^2 = exp(x) / BesselJ(0,2*sqrt(x)).
 * https://oeis.org/A336606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - Σ_{k=1..n} (-1)^k * binom(n,k)^2 * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1                                                            */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache for previously computed terms */
    static seq_t *cache   = NULL;   /* a[0..cache_cap-1]   */
    static size_t  cache_cap = 0;
    static idx_t   cache_size = 0;  /* number of terms stored (<= cache_cap) */

    /* array for factorial values */
    static seq_t *fact   = NULL;    /* fact[i] = i!           */
    static size_t  fact_cap = 0;
    static idx_t   fact_size = 0;    /* number of facts stored */

    /* enlarge cache if necessary */
    if (cache_cap <= n) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }

    /* enlarge factorial array if necessary */
    if (fact_cap <= n) {
        fact_cap = n + 1;
        fact = (seq_t *)realloc(fact, sizeof(seq_t) * fact_cap);
    }

    /* pre‑compute all terms up to n (if not already done) */
    for (idx_t i = cache_size; i <= n; ++i) {

        if (i == 0) {              /* a(0) = 1 */
            cache[0] = (seq_t)1;
            fact[0]  = (seq_t)1;
            continue;
        }

        /* factorial: fact[i] = fact[i-1] * i   */
        fact[i] = fact[i-1] * (seq_t)i;

        /* sums over even and odd k to avoid negative intermediates  */
        seq_t sum_even = 0;
        seq_t sum_odd  = 0;

        /* iteratively compute binomial coefficients binom(n,k) */
        seq_t binom = 1;   /* binom(n,0) = 1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* binom(n,k) = binom(n,k-1) * (n-k+1) / k   */
            binom = binom * (seq_t)(i - k + 1);
            binom = binom / (seq_t)k;

            /* term = binom^2 * a[i-k] */
            seq_t term = cache[i - k];
            term *= binom;
            term *= binom;           /* term is always non–negative */

            if (k & 1) {            /* odd k  → subtract  */
                sum_odd += term;
            } else {                /* even k  → add      */
                sum_even += term;
            }
        }

        /* a[i] = i! + (sum_odd - sum_even) or i! - (sum_even - sum_odd) */
        seq_t an;
        if (sum_odd >= sum_even) {
            an = fact[i] + (sum_odd - sum_even);
        } else {
            an = fact[i] - (sum_even - sum_odd);
        }

        cache[i] = an;
    }

    /* update cache size */
    cache_size = n + 1;

    return cache[n];
}
