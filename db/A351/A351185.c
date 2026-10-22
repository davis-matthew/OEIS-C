/*
 * A351185  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 + 4*x)) / (1 +
 *          4*x).
 * https://oeis.org/A351185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = a(1) = 1; a(n) = Σ_{k=0}^{n-2} binomial(n-2,k) * (-4)^k *
 *            a(n-k-2) (n ≥ 2)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A351185 */
#include <stddef.h>
#include <stdlib.h>


seq_t a(idx_t n) {
    /* ---------- static cache ---------- */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t next_free = 0;   /* first index that has not yet been computed */

    /* ---------- reserve space up to n ---------- */
    size_t need = (size_t)n + 1;
    if (cache_sz < need) {
        size_t new_sz = cache_sz ? cache_sz << 1 : 1;
        while (new_sz < need) new_sz <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        cache = tmp;
        cache_sz = new_sz;
    }

    /* ---------- compute missing terms up to n ---------- */
    for (size_t i = next_free; i < need; ++i) {
        if (i < 2) {
            cache[i] = 1;                     /* a(0) = a(1) = 1 */
        } else {
            seq_t sum  = 0;
            seq_t binom = 1;                 /* binomial(i-2,0) */
            seq_t pow   = 1;                 /* (-4)^0 */
            size_t lim  = i - 2;
            for (size_t k = 0; k <= lim; ++k) {
                sum += binom * pow * cache[i - 2 - k];
                if (k < lim) {
                    /* binom = binom * (lim-k) / (k+1)   (exact division) */
                    binom = binom * (seq_t)(lim - k);
                    binom = binom / (seq_t)(k + 1);
                    /* pow   = pow * (-4) */
                    pow   = pow * (seq_t)(-4);
                }
            }
            cache[i] = sum;
        }
    }
    next_free = need;

    return cache[(size_t)n];
}
