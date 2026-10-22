/*
 * A331611  E.g.f.: exp(1 / (2 - cosh(x)) - 1) (even powers only).
 * https://oeis.org/A331611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A331611 – a(n) = Sum_{k=1..n} binomial(2n-1,2k-1) * A094088(k) * a(n-k),
   a(0)=1.  A094088 itself satisfies A094088(0)=1 and
   A094088(n) = Sum_{k=1..n} binomial(2n,2k) * A094088(n-k).
   This function memoises both sequences and computes them iteratively. */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;
    static seq_t *d_cache = NULL;          /* A094088 values */
    static size_t a_cap = 0;
    static idx_t  a_len  = 0;              /* number of a values stored (a_len == #elements) */

    /* initialise caches on first call */
    if (a_len == 0) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        d_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = 1;   /* a(0) */
        d_cache[0] = 1;   /* A094088(0) */
        a_len   = 1;
    }

    /* grow arrays to needed size */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        d_cache = (seq_t *)realloc(d_cache, new_cap * sizeof(seq_t));
        a_cap   = new_cap;
    }

    /* compute values up to requested n */
    for (idx_t i = a_len; i <= n; ++i) {

        /* --- compute A094088(i) ------------------------------------ */
        seq_t d_val  = 0;
        seq_t bin_full = 1;  /* binomial(2*i,0) */
        for (idx_t k = 1; k <= i; ++k) {
            if (k == 1) {
                bin_full = (seq_t)i * (seq_t)(2 * i - 1);                     /* binomial(2i,2) */
            } else {
                bin_full = bin_full
                           * (seq_t)(2 * i - 2 * k + 1)
                           * (seq_t)(2 * i - 2 * k + 2)
                           / (seq_t)(2 * k)
                           / (seq_t)(2 * k - 1);
            }
            d_val += bin_full * d_cache[i - k];
        }
        d_cache[i] = d_val;

        /* --- compute a(i) ------------------------------------------- */
        seq_t a_val = 0;
        seq_t bin_odd = 0;   /* placeholder for first branch */
        for (idx_t k = 1; k <= i; ++k) {
            if (k == 1) {
                bin_odd = (seq_t)(2 * i - 1);                               /* binomial(2i-1,1) */
            } else {
                bin_odd = bin_odd
                          * (seq_t)(2 * i - 2 * k + 1)
                          * (seq_t)(2 * i - 2 * k + 2)
                          / (seq_t)(2 * k - 1)
                          / (seq_t)(2 * k - 2);
            }
            a_val += bin_odd * d_cache[k] * a_cache[i - k];
        }
        a_cache[i] = a_val;
    }

    a_len = n + 1;
    return a_cache[n];
}
