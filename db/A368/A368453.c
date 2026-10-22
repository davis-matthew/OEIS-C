/*
 * A368453  Expansion of e.g.f. 3*exp(x) / (4 - exp(3*x)).
 * https://oeis.org/A368453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1..n} 3^(k-1) * binomial(n,k) * a(n-k) Initial
 *            value: a(0) = 1 This implementation builds the values on demand
 *            and memoises them in a static array so calls may arrive in any
 *            order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* number of elements allocated */
    static idx_t up_to = 0;      /* highest index that has been computed */

    /* allocate or enlarge the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory, return 0 as a safe failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first element when the cache is first created */
    if (up_to == 0 && cache_cap > 0) {
        cache[0] = (seq_t)1;          /* a(0) = 1 */
    }

    /* compute missing values up to n */
    for (idx_t cur = up_to + 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)cur;     /* C(cur,1) */
        seq_t pow3  = (seq_t)1;       /* 3^(k-1) with k=1 */

        for (idx_t k = 1; k <= cur; ++k) {
            /* term = 3^(k-1) * C(cur,k) * a(cur-k) */
            seq_t term = binom * pow3;
            term = term * cache[cur - k];
            sum += term;

            if (k < cur) {
                /* update binom to C(cur,k+1):
                   C(cur,k+1) = C(cur,k) * (cur-k)/(k+1) */
                binom = (binom * ((seq_t)(cur - k))) / ((seq_t)(k + 1));
                pow3 *= (seq_t)3;
            }
        }
        cache[cur] = (seq_t)1 + sum;
    }
    up_to = n > up_to ? n : up_to;

    return cache[n];
}
