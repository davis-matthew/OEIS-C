/*
 * A334565  E.g.f.: exp(-(x + x^2 + x^3 + x^4 + x^5)).
 * https://oeis.org/A334565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 21, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334565
   Formula: a(0) = 1
            a(n) = - (n-1)! * Σ_{k=1..min(5,n)} [ k * a(n-k) / (n-k)! ]   (n≥1)   */

#include <stdlib.h>

/* local cache for memoisation */
static seq_t *cache = NULL;   /* a[0]..a[last]   */
static idx_t  cap   = 0;      /* current capacity of cache   */
static idx_t  last  = (idx_t)-1;  /* highest index computed so far */

/* grow cache to hold indices 0..n (inclusive) */
static void grow_cache(idx_t n)
{
    if (cap <= n) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        /* cast needed for valid C++ code */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = (idx_t)new_cap;
    }
}

seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (cache == NULL) {
        /* allocate one element for a(0) */
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        cache[0] = 1;          /* a(0) = 1 */
        cap   = 1;
        last  = 0;
    }

    /* compute any missing terms up to n */
    if (n > last) {
        grow_cache(n);
        for (idx_t m = last + 1; m <= n; ++m) {
            seq_t sum = 0;
            idx_t maxk = (m < 5 ? m : 5);
            for (idx_t k = 1; k <= maxk; ++k) {
                /* (m-1)!/(m-k)! = product of (m-k+1)..(m-1)  (empty product => 1) */
                seq_t mult = 1;
                for (idx_t i = m - k + 1; i < m; ++i)
                    mult *= (seq_t)i;
                seq_t term = cache[m - k];
                term *= (seq_t)k;
                term *= mult;
                sum += term;
            }
            cache[m] = -sum;
        }
        last = n;
    }
    return cache[n];
}
