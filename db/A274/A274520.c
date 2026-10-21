/*
 * A274520  a(n) = ((1 + sqrt(7))^n - (1 - sqrt(7))^n)/sqrt(7).
 * https://oeis.org/A274520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((1 + sqrt(7))^n - (1 - sqrt(7))^n)/sqrt(7); first terms:
 *            a(0) = 0, a(1) = 2; recurrence: a(n) = 2*a(n-1) + 6*a(n-2) for n
 *            >= 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of elements already stored   */

    /* initialise cache with first two terms if not yet done */
    if (cached == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;          /* abort gracefully on allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cached = 2;
    }

    /* if requested index already computed, return it */
    if (n < cached) return cache[n];

    /* expand cache to hold index n */
    idx_t newsize = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp) return 0;                  /* allocation failure → return 0 */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cached; i <= n; ++i) {
        if (i == 0) { cache[i] = (seq_t)0; }
        else if (i == 1) { cache[i] = (seq_t)2; }
        else {
            cache[i] = (seq_t)( (seq_t)2 * cache[i-1] + (seq_t)6 * cache[i-2] );
        }
    }
    cached = newsize;
    return cache[(size_t)n];
}
