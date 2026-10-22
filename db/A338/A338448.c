/*
 * A338448  E.g.f.: 1 / (1 - x - log(1 - x)).
 * https://oeis.org/A338448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 14, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence:  a(0)=1, a(1)=0
 *               a(n) = - Σ_{k=2..n} binomial(n,k) * (k-1)! * a(n-k)
 *   (All operations are in the integer type seq_t, which may be a
 *   signed integer of any size or an arbitrary-precision integer class.)
 *
 * The function memoises already computed values in static arrays,
 * so repeated calls and calls in arbitrary order are fast.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* a[n] values */
static seq_t *fact  = NULL;          /* (k-1)!   */
static size_t cache_len = 0;         /* allocated length */

/* ---- Ensure that cache and factorial arrays are large enough ---- */
static void ensure_capacity(size_t n) {
    if (n + 1 <= cache_len) return;               /* already enough */

    size_t new_len = n + 1;

    /* resize caches */
    cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    fact  = (seq_t*)realloc(fact,  new_len * sizeof(seq_t));

    /* initialise factorials for indices that were not yet computed */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i == 0)          fact[i] = (seq_t)1;     /* 0!  */
        else if (i == 1)     fact[i] = (seq_t)1;     /* 1!  */
        else                 fact[i] = fact[i-1] * (seq_t)(i-1); /* (i-1)! */
    }

    /* compute new a[n] values */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i == 0) { cache[i] = (seq_t)1; continue; }
        if (i == 1) { cache[i] = (seq_t)0; continue; }

        seq_t sum = (seq_t)0;
        /* start with k = 2 */
        seq_t binom = (seq_t)i * (seq_t)(i-1) / (seq_t)2; /* C(i,2) */

        for (size_t k = 2; k <= i; ++k) {
            if (k > 2) {
                /* C(i,k) = C(i,k-1) * (i-k+1) / k */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
            }
            /* term = binom * (k-1)! * a[i-k] */
            seq_t term = binom * fact[k] * cache[i - k];
            sum += term;
        }
        cache[i] = -sum;
    }

    cache_len = new_len;
}

/* ---- Public sequence function ---- */
seq_t a(idx_t n)
{
    ensure_capacity((size_t)n);
    return cache[(size_t)n];
}
