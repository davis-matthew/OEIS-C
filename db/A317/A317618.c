/*
 * A317618  Expansion of e.g.f. sqrt((1 - x)/(1 - 3*x)).
 * https://oeis.org/A317618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A317618: a(n) = Sum_{k=0..n} binomial(n-1,k-1)*(2*k-1)!!*n!/k! */
/* D‑finite recurrence (gives a(0)=1, a(1)=1):
      a(n+2) = (5 + 4*n)*a(n+1) - (3*n^2 + 3*n)*a(n)   for n >= 0  */

/* The function keeps a memoised cache, so that repeated calls for
   arbitrary n (not necessarily increasing) are fast.  All arithmetic
   is performed with the type 'seq_t', which is defined by the build
   system.  The cache is allocated with realloc and grows only as
   necessary.  The implementation works for every seq_t width, up
   to the point where the final value fits; for the 64‑bit build it
   remains accurate for all n whose a(n) < 2^64. */

#include <stdlib.h>     /* malloc, realloc, free */
#include <stdint.h>     /* for clarity of types but not required */

/* External linkage */
seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* allocated length of cache   */
    static idx_t  computed_max = 1;/* highest index that has been
                                      computed (a[0] and a[1] are known) */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 2;                 /* at least a[0] and a[1] */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* enlarge cache if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing terms up to n (if any) */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            idx_t k = i - 2;                           /* coefficient index */
            seq_t term1 = (seq_t)(5 + 4 * k) * cache[i - 1];
            seq_t term2 = (seq_t)(3 * k * k + 3 * k) * cache[i - 2];
            cache[i] = term1 - term2;                  /* recurrence */
        }
        computed_max = n;
    }

    return cache[n];
}
