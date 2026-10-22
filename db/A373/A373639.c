/*
 * A373639  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-5*k,k).
 * https://oeis.org/A373639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - a(n-6) for n > 5 Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=6
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 45, u64 n <= 89, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)2;
        case 4: return (seq_t)4;
        case 5: return (seq_t)6;
        default: break;
    }

    /* Static cache to store previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Ensure enough space */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        if (!cache) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!cache) { /* allocation failure – return 0 as a safe default */
            return (seq_t)0;
        }
        cache_cap = new_cap;
    }

    /* Initialize base values if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)6;
        cache_len = 6;
    }

    /* If we already have computed up to n, return */
    if (n < cache_len)
        return cache[n];

    /* Compute terms up to n using the recurrence */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t term = cache[i-1] + (seq_t)2 * cache[i-3] - cache[i-6];
        cache[i] = term;
    }
    cache_len = n + 1;      /* now we have values up to n inclusive */
    return cache[n];
}
