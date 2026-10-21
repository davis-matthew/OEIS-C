/*
 * A261594  G.f.: sqrt( Sum_{n>=0} x^n * Sum_{k=0..n} C(n,k) * 2^(k*(k-1)) ).
 * https://oeis.org/A261594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in sqrt( Σ_{m≥0} x^m * Σ_{k=0..m}
 *            C(m,k) * 2^(k(k-1)) )
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_comp = 0;

    /* initialise cache if this is the first call */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)1;              /* a(0) = 1 */
    }

    /* grow cache to accommodate index n */
    while (cap <= (size_t)n) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* already computed? */
    if (n <= max_comp) return cache[n];

    /* compute terms up to n */
    for (idx_t idx = max_comp + 1; idx <= n; ++idx) {
        /* B(idx) = Σ_{k=0..idx} C(idx,k) * 2^(k(k-1)) */
        seq_t B = 0;
        seq_t binom = 1;          /* C(idx,0) */
        seq_t pow = 1;            /* current 2^(k(k-1)) */
        seq_t factor = 1;         /* 2^(2k) for the next step */
        for (idx_t k = 0; k <= idx; ++k) {
            B += binom * pow;
            if (k < idx) {
                binom = binom * (seq_t)(idx - k) / (seq_t)(k + 1);
                pow   = pow * factor;
                factor = factor * 4;
            }
        }

        /* convolution sum Σ_{i=1..idx-1} a(i)*a(idx-i) */
        seq_t sum = 0;
        for (idx_t i = 1; i < idx; ++i) {
            sum += cache[i] * cache[idx - i];
        }

        /* a(idx) = (B - sum)/2 */
        cache[idx] = (B - sum) / 2;
    }

    max_comp = n;
    return cache[n];
}
