/*
 * A368443  Expansion of e.g.f. exp(-x) / (4 - 3*exp(2*x)).
 * https://oeis.org/A368443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + 3 * Sum_{k=1..n} 2^k * binomial(n,k) * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*---  memoisation  -----------------------------------------------------*/
    static seq_t *cache = NULL;   /* cache[i] = a(i) */
    static idx_t  cache_cap = 0;  /* capacity of cache array */
    static idx_t  next_computed = 0; /* next index that needs to be computed */

    static seq_t *pow2 = NULL;    /* pow2[k] = 2^k */
    static idx_t  pow2_cap = 0;    /* capacity of pow2 array */

    static seq_t *binom = NULL;   /* binom[k] = C(m,k) for the current row m */
    static idx_t  binom_cap = 0;   /* capacity of binom array */

    /*---  ensure cache capacity  ------------------------------------------*/
    if (n + 1 > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        cache = tmp;
        cache_cap = n + 1;
    }

    /*---  initialise a(0)  ----------------------------------------------*/
    if (next_computed == 0) {
        cache[0] = (seq_t)1;
        next_computed = 1;
    }

    /*---  ensure pow2 capacity  ------------------------------------------*/
    if (n + 1 > pow2_cap) {
        seq_t *tmp = (seq_t *)realloc(pow2, sizeof(seq_t) * (n + 1));
        pow2 = tmp;
        if (pow2_cap == 0) {
            pow2[0] = (seq_t)1;
            for (idx_t i = 1; i <= n; ++i)
                pow2[i] = pow2[i - 1] * ((seq_t)2);
        } else {
            for (idx_t i = pow2_cap; i <= n; ++i)
                pow2[i] = pow2[i - 1] * ((seq_t)2);
        }
        pow2_cap = n + 1;
    }

    /*---  ensure binom capacity  -----------------------------------------*/
    if (n + 1 > binom_cap) {
        seq_t *tmp = (seq_t *)realloc(binom, sizeof(seq_t) * (n + 1));
        binom = tmp;
        for (idx_t i = binom_cap; i <= n; ++i)
            binom[i] = (seq_t)0;
        binom_cap = n + 1;
    }

    /*---  initial row (m = 0)  -------------------------------------------*/
    if (binom[0] != (seq_t)1)
        binom[0] = (seq_t)1;

    /*---  compute all missing terms up to n  -----------------------------*/
    for (idx_t m = next_computed; m <= n; ++m) {
        /* update binomial coefficients for row m: C(m,k) = C(m-1,k) + C(m-1,k-1) */
        for (idx_t k = m; k >= 1; --k)
            binom[k] = binom[k] + binom[k - 1];
        binom[0] = (seq_t)1;

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = binom[k];
            term = term * pow2[k];
            term = term * cache[m - k];
            sum = sum + term;
        }

        sum = sum * ((seq_t)3);
        if ((m & 1) == 0)       /* even m : +1  */
            sum = sum + ((seq_t)1);
        else                    /* odd m  : -1  */
            sum = sum - ((seq_t)1);

        cache[m] = sum;
    }

    next_computed = n + 1;
    return cache[n];
}
