/*
 * A380307  Expansion of e.g.f. exp( (1+5*x)^(1/5) - 1 ).
 * https://oeis.org/A380307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1..n} (∏_{j=0..k-1} (−5·j+1)) · binomial(n−1,k−1) ·
 *            a(n−k), with a(0) = 1
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Static cache for a and for the product p[k] = ∏_{j=0..k-1}(−5·j+1). */
    static seq_t *a_cache = NULL;
    static seq_t *p_cache = NULL;
    static size_t a_cap   = 0;  /* allocated capacity for a_cache */
    static size_t p_cap   = 0;  /* allocated capacity for p_cache */
    static idx_t  a_len   = 0;  /* number of values already computed in a_cache   */
    static idx_t  p_len   = 0;  /* number of products already computed in p_cache */

    /* Allocate (or grow) the caches to hold up to n (inclusive). */
    if (a_cache == NULL) {                 /* first call: allocate default size */
        size_t init = 16;
        if (init < (size_t)(n + 1))
            init = n + 1;
        a_cache = (seq_t *)malloc(init * sizeof(seq_t));
        p_cache = (seq_t *)malloc(init * sizeof(seq_t));
        a_cap   = init;
        p_cap   = init;

        /* initialise base case */
        a_cache[0] = (seq_t)1;
        p_cache[0] = (seq_t)1;
        a_len = 1;
        p_len = 1;
    } else if (a_cap <= (size_t)(n + 1)) { /* grow if needed */
        size_t new_cap = a_cap << 1;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        p_cache = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
        a_cap   = new_cap;
        p_cap   = new_cap;
    }

    /* Compute missing products p[k] up to n. */
    for (idx_t i = p_len; i <= n; ++i) {
        seq_t factor = ((seq_t)1 - (seq_t)(5 * (i - 1)));
        p_cache[(size_t)i] = p_cache[(size_t)(i - 1)] * factor;
    }
    p_len = n + 1;

    /* Compute missing a[n] values from a_len up to n. */
    for (idx_t cur = a_len; cur <= n; ++cur) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;                 /* binomial(cur-1,0) */

        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term = p_cache[(size_t)k] * binom * a_cache[(size_t)(cur - k)];
            sum += term;
            if (k < cur) {                     /* update binomial for next k */
                seq_t nm = (seq_t)(cur - k);   /* numerator term */
                seq_t dn = (seq_t)k;           /* denominator term */
                binom = (binom * nm) / dn;
            }
        }
        a_cache[(size_t)cur] = sum;
    }
    a_len = n + 1;

    return a_cache[(size_t)n];
}
