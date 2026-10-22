/*
 * A367924  Expansion of e.g.f. 1/(3 - x - 2*exp(x)).
 * https://oeis.org/A367924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A367924
 * Formula (recurrence):
 *   a(0) = 1
 *   a(n) = n * a(n-1) + 2 * sum_{k=1..n} binomial(n,k) * a(n-k),   n >= 1
 *
 * The implementation memoises computed terms in a static array and
 * uses the multiplicative formula for binomial coefficients to keep
 * the algorithm O(n²) time and O(n) space.
 */

#include <stdlib.h>
/* idx_t and seq_t are defined by the build system; do not redeclare. */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;  /* array of a(i) values */
    static idx_t cache_len   = 0;     /* number of values stored (next free index) */
    static idx_t cache_cap   = 0;     /* allocated capacity */

    /* initialise cache for the first call */
    if (cache_len == 0) {
        cache_cap = 1;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                 /* allocation failure */
        cache[0]  = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
    }

    /* extend cache if the requested index is beyond what we have */
    if (n >= cache_len) {
        idx_t new_cap = n + 1;
        if (new_cap < 2 * cache_cap)
            new_cap = 2 * cache_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* compute PartialB = sum_{k=0..i-1} C(i,k) * a(k) */
            seq_t sum = 0;
            seq_t bin = (seq_t)1;           /* C(i,0) */
            sum += bin * cache[0];          /* k = 0 */

            for (idx_t k = 1; k < i; ++k) {
                /* C(i,k) = C(i,k-1) * (i-k+1) / k  */
                bin = bin * (seq_t)(i - k + 1) / (seq_t)k;
                sum += bin * cache[k];
            }

            /* a(i) = i * a(i-1) + 2 * sum_{k=0..i-1} C(i,k) * a(k) */
            cache[i] = (seq_t)i * cache[i - 1] + (seq_t)2 * sum;
        }

        cache_len = n + 1;
    }

    return cache[n];
}
