/*
 * A400529  E.g.f. A(x) satisfies A(x) = exp(2*x*A(x^2)).
 * https://oeis.org/A400529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A400529:  a(n) = 2*(n-1)! * Σ_{k=0..⌊(n-1)/2⌋} (2k+1) *
      a(k) * a(n-1-2k) / (k! * (n-1-2k)!)
   a(0) = 1.
*/

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* array of computed terms                   */
static size_t cache_size = 0;    /* allocated size (max index + 1)           */
static idx_t  last_computed = (idx_t)-1;  /* highest index computed so far   */

/* compute a(n) using the recurrence, with memoisation */
seq_t a(idx_t n) {
    /* initialise cache with a(0) = 1 */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* expand cache if needed */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        if (new_size < cache_size * 2) new_size = cache_size * 2;  /* optional growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    idx_t start = (last_computed == (idx_t)-1) ? 0 : last_computed + 1;
    if (start < 1) start = 1;   /* a(0) already known */

    for (idx_t m = start; m <= n; ++m) {
        if (m == 0) {           /* already set */
            cache[0] = (seq_t)1;
            continue;
        }
        idx_t r = m - 1;                      /* r = n-1 */
        idx_t maxk = r / 2;                   /* floor((n-1)/2) */
        seq_t sum = (seq_t)0;
        seq_t T = (seq_t)1;                    /* T = r!/(k! * (r-2k)!), k=0 -> 1 */

        for (idx_t k = 0; k <= maxk; ++k) {
            /* term = T * (2k+1) * a(k) * a(r-2k) */
            seq_t term = T;
            term = term * (seq_t)(2 * k + 1);
            term = term * cache[k];
            term = term * cache[r - 2 * k];
            sum += term;

            /* update T for next k: T *= (r-2k)*(r-2k-1)/(k+1) */
            if (k < maxk) {
                seq_t num1 = r - 2 * k;
                seq_t num2 = r - 2 * k - 1;
                seq_t den  = k + 1;
                T = (T * num1 * num2) / den;
            }
        }
        cache[m] = sum * (seq_t)2;   /* outer factor 2 */
    }

    last_computed = n;
    return cache[n];
}
