/*
 * A251689  G.f. satisfies: A(x) = exp( Sum_{n>=1} [Sum_{k=0..2*n}
 *          A200536(n,2*n-k)^2 * x^k] / A(x)^n * x^n/n ), where
 *          A200536(n,2*n-k) is the coefficient of x^k in (2+3*x+x^2)^n.
 * https://oeis.org/A251689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = (1+4x)(1+4x^2)(1+x^2)(1+x^3)/(1-2x^2)^2. Thus the
 *            coefficients a(n) obey a(n) = 4*a(n-2) - 4*a(n-4) + p(n), where
 *            p(n) = coefficient of x^n in (1+4x)(1+4x^2)(1+x^2)(1+x^3)
 *            (p(0..8)= {1,4,5,21,8,21,20,4,16}, p(n)=0 for n>8)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Coefficients of the numerator polynomial P(x). */
static const seq_t p_coeff[9] = {
    (seq_t)1,
    (seq_t)4,
    (seq_t)5,
    (seq_t)21,
    (seq_t)8,
    (seq_t)21,
    (seq_t)20,
    (seq_t)4,
    (seq_t)16
};

seq_t a(idx_t n)
{
    /* Allocate or grow cache if needed. */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – return 0 as a safe fall‑back */
            return (seq_t)0;
        }
        cache = new_cache;
        /* Compute only the new entries. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 2) val += (seq_t)4 * cache[i - 2];
            if (i >= 4) val -= (seq_t)4 * cache[i - 4];
            if (i <= 8) val += p_coeff[i];
            cache[i] = val;
        }
        cache_len = new_len;
    }
    return cache[n];
}
