/*
 * A265059  Coordination sequence for (2,3,9) tiling of hyperbolic plane.
 * https://oeis.org/A265059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-5)+a(n-8)-a(n-10) (n ≥ 11) Initial values:
 *            a(0)=1, a(1)=3, a(2)=5, a(3)=7, a(4)=9, a(5)=12, a(6)=16,
 *            a(7)=21, a(8)=28, a(9)=36, a(10)=45
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 89, u64 n <= 184,
 *            u128 n <= 375, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Coefficients of the numerator N(x) for 0 ≤ k ≤ 10;
   N(x) = (x+1)^2*(x^2+x+1)*(x^6+x^3+1) = Σ_{k=0}^{10} c_k x^k    */
static const unsigned char coef[11] = {1,3,4,4,4,4,4,4,4,3,1};

seq_t a(idx_t n)
{
    /* cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;

    /* grow cache to at least n+1 elements if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;     /* allocation failed – return 0 as fallback */
        cache = tmp;

        /* compute missing terms from cache_len up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t val = 0;

            if (i >= 2) val += cache[i-2];
            if (i >= 5) val += cache[i-5];
            if (i >= 8) val += cache[i-8];
            if (i >= 10) val -= cache[i-10];   /* minus comes from +x^10 term in D(x) */

            if (i <= 10) val += (seq_t)coef[i];     /* add the constant from N(x) */

            cache[i] = val;
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
