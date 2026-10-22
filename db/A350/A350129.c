/*
 * A350129  a(0) = 0; thereafter a(n) = a(n-1)/2 + n if a(n-1) is even,
 *          otherwise a(n) = a(n-1) + a(n-2).
 * https://oeis.org/A350129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; thereafter a(n) = a(n-1)/2 + n if a(n-1) is even, a(n)
 *            = a(n-1) + a(n-2) if a(n-1) is odd
 * Range:     u8 n <= 16, u16 n <= 40, u32 n <= 88, u64 n <= 184,
 *            u128 n <= 376, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for 0 <= i <= last */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;          /* last computed index (initially 0) */

    /* Allocate cache on first call */
    if (cache == NULL) {
        cap = 1;                    /* at least entry 0 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)          /* allocation failure: return 0 */
            return (seq_t)0;
        cache[0] = (seq_t)0;
    }

    /* Increase capacity if needed */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap)
            newcap = (newcap ? newcap * 2 : 4);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL)           /* allocation failure: return 0 */
            return (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* Compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        const seq_t prev = cache[i - 1];
        if ((prev & (seq_t)1) == 0)               /* a(i-1) even */
            cache[i] = (prev >> 1) + (seq_t)i;
        else                                     /* a(i-1) odd  */
            cache[i] = prev + cache[i - 2];
    }
    last = n;

    return cache[n];
}
