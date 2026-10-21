/*
 * A295336  Numerators of the convergents to sqrt(14)/2 = A294969.
 * https://oeis.org/A295336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 25, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295336: Numerators of the convergents to sqrt(14)/2
   Recurrence: a(n) = 30*a(n-4) - a(n-8)   for n >= 8
   Initial values:
     a(0)=1,  a(1)=2,  a(2)=13, a(3)=15,
     a(4)=43, a(5)=58, a(6)=391, a(7)=449
   The implementation memoises the sequence so that calls can arrive
   in any order.  It works for all supported seq_t types. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: values up to cache_len-1 are stored. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* current number of stored elements */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Grow the cache if needed to hold index n. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while ((size_t)n >= new_cap) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 7) {                    /* initial values */
            switch (i) {
            case 0: cache[i] = (seq_t)1;            break;
            case 1: cache[i] = (seq_t)2;            break;
            case 2: cache[i] = (seq_t)13;           break;
            case 3: cache[i] = (seq_t)15;           break;
            case 4: cache[i] = (seq_t)43;           break;
            case 5: cache[i] = (seq_t)58;           break;
            case 6: cache[i] = (seq_t)391;          break;
            case 7: cache[i] = (seq_t)449;          break;
            }
        } else {                          /* recurrence */
            cache[i] = (seq_t)30 * cache[i-4] - cache[i-8];
        }
    }
    cache_len = (size_t)n + 1;   /* update stored length */

    return cache[n];
}
