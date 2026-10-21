/*
 * A292490  p-INVERT of the odd positive integers, where p(S) = 1 - S - 7 S^2.
 * https://oeis.org/A292490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 17*a(n-3) + 7*a(n-4) for n>=5 Initial terms:
 *            a(0)=1, a(1)=11, a(2)=68, a(3)=365, a(4)=2019
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t   *cache   = NULL;  /* pointer to array of terms */
    static idx_t   cap      = 0;     /* capacity of the array */
    static idx_t   sz       = 0;     /* number of terms currently stored */

    /* Initialisation of the cache */
    if (cache == NULL) {
        cap = 8;                                         /* start with 8 slots */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                     /* allocation failed */
        /* set the first 5 terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)68;
        cache[3] = (seq_t)365;
        cache[4] = (seq_t)2019;
        sz = 5;
    }

    /* Extend cache if necessary */
    if (n >= sz) {
        /* Ensure we have enough storage */
        if (n >= cap) {
            idx_t new_cap = cap;
            while (new_cap <= n) new_cap <<= 1;           /* double until large enough */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;                     /* reallocation failed */
            cache = tmp;
            cap = new_cap;
        }
        /* Compute missing terms up to n */
        for (idx_t i = sz; i <= n; ++i) {
            if (i < 5) {
                /* already initialised */}
            else {
                cache[i] = 5 * cache[i-1] + 17 * cache[i-3] + 7 * cache[i-4];
            }
        }
        sz = n + 1;
    }
    return cache[n];
}
