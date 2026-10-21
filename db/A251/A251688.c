/*
 * A251688  G.f. satisfies: A(x) = exp( Sum_{n>=1} [Sum_{k=0..2*n} T(n,k)^2 *
 *          x^k] / A(x)^n * x^n/n ), where T(n,k) is the coefficient of x^k in
 *          (1+2*x)^n*(1+3*x)^n.
 * https://oeis.org/A251688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A251688: G.f. (1+x)*(1+4*x^2)*(1+9*x^2)*(1+36*x^3) / (1-6*x^2)^2
   The coefficients satisfy the linear recurrence
      a(n) = 12*a(n-2) - 36*a(n-4) + p(n)
   where p(n) is the coefficient of x^n in the numerator polynomial:
   p = {1, 1, 13, 49, 72, 504, 468, 1296, 1296} for n = 0..8, and p(n)=0 for n>8. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* coefficients p(n) for the numerator polynomial */
    static const seq_t p[9] = { (seq_t)1, (seq_t)1, (seq_t)13, (seq_t)49,
                                (seq_t)72, (seq_t)504, (seq_t)468,
                                (seq_t)1296, (seq_t)1296 };

    /* increase cache size if necessary */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {               /* allocation failure – return 0 */
            return (seq_t)0;
        }
        cache = tmp;

        /* compute missing terms from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = 0;
            if (i <= 8)         val += p[i];
            if (i >= 2)         val += (seq_t)12 * cache[i - 2];
            if (i >= 4)         val -= (seq_t)36 * cache[i - 4];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
