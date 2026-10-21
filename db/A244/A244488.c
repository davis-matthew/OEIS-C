/*
 * A244488  G.f.:
 *          (1+x^2+3*x^3+4*x^4+4*x^5+4*x^6+3*x^7+x^8+x^10)/((1-x^2)*(1-x^3)*(1-x^4)*(1-x^5)).
 * https://oeis.org/A244488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G(x) = (1 + x^2 + 3*x^3 + 4*x^4 + 4*x^5 + 4*x^6 + 3*x^7 + x^8 +
 *            x^10) /*
 *            --------------------------------------------------------------------
 *            * / (1-x^2)(1-x^3)(1-x^4)(1-x^5)
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifndef SEQ_T_DEFINED
/* The type seq_t is supplied by the build; do not define it here. */
#endif

/* exported function */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* helper arrays: denominator coefficients d_k   (k = 0..14)           */
/* and numerator coefficients N_k   (k = 0..10)                       */
static const int D[15] = {
    1,  /* x^0  */
    0,  /* x^1  */
   -1,  /* x^2  */
   -1,  /* x^3  */
   -1,  /* x^4  */
    0,  /* x^5  */
    1,  /* x^6  */
    2,  /* x^7  */
    1,  /* x^8  */
    0,  /* x^9  */
   -1,  /* x^10 */
   -1,  /* x^11 */
   -1,  /* x^12 */
    0,  /* x^13 */
    1   /* x^14 */
};

static const int N_coef[11] = {
    1,  /* x^0  */
    0,  /* x^1  */
    1,  /* x^2  */
    3,  /* x^3  */
    4,  /* x^4  */
    4,  /* x^5  */
    4,  /* x^6  */
    3,  /* x^7  */
    1,  /* x^8  */
    0,  /* x^9  */
    1   /* x^10 */
};

/* ------------------------------------------------------------------ */
/* static state: a cache of computed terms                           */
static seq_t *cache = NULL;           /* array holding a(0)..a(max_computed)   */
static size_t cap   = 0;              /* allocated capacity (number of elems)  */
static idx_t  max_computed = (idx_t)(-1); /* highest n already computed, -1 means none */

/* ------------------------------------------------------------------ */
/* ensure that cache can hold indices 0 .. n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        cap = (size_t)n + 1;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) abort();
    } else if (cap <= (size_t)n) {
        cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
}

/* ------------------------------------------------------------------ */
/* compute all values a(k) for k = start … n (inclusive) */
static void compute_upto(idx_t n)
{
    if (max_computed != (idx_t)(-1) && max_computed >= n) return;

    ensure_capacity(n);

    idx_t start = (max_computed == (idx_t)(-1)) ? 0 : max_computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t sum = (i < 11) ? (seq_t)N_coef[i] : (seq_t)0;

        /* subtract contributions from previous terms:
           a(i) = N[i] - Σ d_k * a(i-k)  (k>=1)                */
        for (int k = 1; k <= 14; ++k) {
            if (i >= (idx_t)k) {
                if (D[k] < 0)
                    sum += (seq_t)(-D[k]) * cache[(size_t)(i - (idx_t)k)];
                else
                    sum -= (seq_t)D[k] * cache[(size_t)(i - (idx_t)k)];
            }
        }
        cache[(size_t)i] = sum;
    }
    max_computed = n;
}

/* ------------------------------------------------------------------ */
/* exported function that returns the nth term of the sequence */
seq_t a(idx_t n)
{
    compute_upto(n);
    return cache[(size_t)n];
}
