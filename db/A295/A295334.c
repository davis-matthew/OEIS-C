/*
 * A295334  Denominators of continued fraction convergents to sqrt(10)/2 =
 *          sqrt(5/2) = A020797 + 1.
 * https://oeis.org/A295334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 74, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295334 – Denominators of convergents to sqrt(10)/2.
   Recurrence: a(n) = 6*a(n-3) + a(n-6)   for n ≥ 6
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=7, a(5)=12
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  cache_used = number of terms computed.  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_used = 0;  /* number of valid elements */

    /* Initialise with the first six terms if necessary. */
    if (cache_used == 0) {
        cache_cap = 6;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)12;
        cache_used = 6;
    }

    size_t idx = (size_t)n;

    /* Return cached value if already computed. */
    if (idx < cache_used)
        return cache[idx];

    /* Ensure the array is large enough to hold up to idx. */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the missing terms using the recurrence. */
    for (size_t i = cache_used; i <= idx; ++i) {
        cache[i] = (seq_t)6 * cache[i - 3] + cache[i - 6];
    }

    cache_used = idx + 1;
    return cache[idx];
}
