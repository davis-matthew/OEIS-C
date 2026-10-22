/*
 * A355268  a(n) = n! * [x^n] -exp(x^2)/(x - 1).
 * https://oeis.org/A355268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/2⌋} 1/k!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n*a(n-1) + 2*(n-1)*a(n-2) - 2*(n-1)*(n-2)*a(n-3) */
/* Initial values: a(0)=1, a(1)=1, a(2)=4 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* allocated capacity */
    static idx_t  last    = 0;   /* largest index already computed */

    /* Handle the first three terms directly (no allocation needed) */
    if (n < 3) {
        switch ((int)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)4;
            default: return (seq_t)0; /* unreachable */
        }
    }

    /* Allocate/expand cache as required */
    if (n >= cap) {
        size_t newCap = cap ? cap << 1 : 8;
        while (newCap <= (size_t)n) newCap <<= 1;
        cache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */

        /* Initialise base values if this is the first allocation */
        if (last == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)4;
            last = 2;
        }
        cap = newCap;
    }

    /* Compute any values that are still missing */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)i * cache[i-1];
        seq_t term2 = (seq_t)2 * (seq_t)(i-1) * cache[i-2];
        seq_t term3 = (seq_t)2 * (seq_t)(i-1) * (seq_t)(i-2) * cache[i-3];
        cache[i] = term1 + term2 - term3;
    }
    last = n;
    return cache[n];
}
