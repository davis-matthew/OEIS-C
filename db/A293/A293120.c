/*
 * A293120  Expansion of e.g.f. exp(x^2/(1+x)).
 * https://oeis.org/A293120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = exp(x^2/(1+x)) (e.g.f.). The sequence satisfies d(0)=1,
 *            d(1)=0 and for n>=2 d(n) = sum_{k=2}^{n} binom(n-1,k-1) * k! *
 *            d(n-k). The desired terms are a(n)=(-1)^n * d(n)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a straightforward O(n^2) recurrence with
   memoisation so that multiple calls to a(idx_t n) with arbitrary
   values of n up to about 10000 are answered quickly. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static caches for factorials and d[n] values */
    static seq_t *fact = NULL;       /* factorials: fact[i] = i!   */
    static seq_t *d   = NULL;        /* d[i] holds A052845(i)      */
    static idx_t last_computed = 0;  /* max index for which d has been set   */
    static size_t fact_cap = 0;
    static size_t cache_cap = 0;

    /* enlarge factorial cache if needed */
    if (n + 1 > fact_cap) {
        size_t new_cap = fact_cap == 0 ? 2 : fact_cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        fact = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
        /* initialise new entries */
        for (size_t i = fact_cap; i < new_cap; ++i) {
            if (i == 0) fact[i] = 1;
            else fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }

    /* enlarge d cache if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 2 : cache_cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        d = (seq_t*)realloc(d, new_cap * sizeof(seq_t));
        /* initialise new entries to zero */
        for (size_t i = cache_cap; i < new_cap; ++i) d[i] = 0;
        cache_cap = new_cap;
    }

    /* initialise base cases (only once) */
    if (cache_cap >= 1 && d[0] == 0) d[0] = 1;
    if (cache_cap >= 2 && d[1] == 0) d[1] = 0;

    /* compute from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i < 2) continue;   /* base cases already handled */
        seq_t sum = 0;
        seq_t binom = (seq_t)(i - 1);     /* binom(i-1,1) for k=2 */
        for (idx_t k = 2; k <= i; ++k) {
            seq_t term = binom * fact[k];
            term *= d[i - k];
            sum += term;
            if (k < i) {
                /* Update binom to binom(i-1,k) */
                binom = binom * (seq_t)(i - k) / (seq_t)k;
            }
        }
        d[i] = sum;
    }
    last_computed = n >= last_computed ? n : last_computed;

    seq_t result = d[n];
    if (n & 1) result = -result;   /* a(n) = (-1)^n * d(n) */
    return result;
}
