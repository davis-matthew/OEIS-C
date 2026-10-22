/*
 * A361598  Expansion of e.g.f. exp( x/(1-x)^2 ) / (1-x).
 * https://oeis.org/A361598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n - 1)*a(n-1) - (n-1)*(3*n - 5)*a(n-2) +
 *            (n-2)^2*(n-1)*a(n-3), with initial values a(0)=1, a(1)=2,
 *            a(2)=9. This recurrence was derived from the generating‑function
 *            definition of A361598
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed values */
    static size_t  cap   = 0;     /* allocated capacity (in elements)      */
    static idx_t   last  = 0;     /* last index that has been computed */
    /* Initialise cache on first call */
    if (cache == NULL) {
        cap  = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory – return 0 */
        cache[0] = 1;                         /* a(0) = 1 */
        last   = 0;
    }
    /* Ensure we have enough space for index n */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory – return 0 */
        cap = new_cap;
    }
    /* Compute values up to n if we have not already */
    if (n > last) {
        idx_t i;
        for (i = last + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = 2;                       /* a(1) */
            } else if (i == 2) {
                cache[2] = 9;                       /* a(2) */
            } else {
                /* Term = (3*i-1)*a[i-1] + (i-2)^2*(i-1)*a[i-3] - (i-1)*(3*i-5)*a[i-2] */
                seq_t term = ((seq_t)(3 * i - 1)) * cache[i - 1];
                term += ((seq_t)(i - 2)) * ((seq_t)(i - 2))
                      * ((seq_t)(i - 1)) * cache[i - 3];
                term -= ((seq_t)(i - 1)) * ((seq_t)(3 * i - 5))
                      * cache[i - 2];
                cache[i] = term;
            }
        }
        last = n;
    }
    return cache[n];
}
