/*
 * A242563  a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4), a(0)=a(1)=0, a(2)=2, a(3)=3.
 * https://oeis.org/A242563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4) with a(0)=0, a(1)=0, a(2)=2,
 *            a(3)=3
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* stored values of the sequence   */
    static size_t   cap    = 0;    /* allocated size (in terms)       */
    static idx_t   computed = 0;   /* number of terms already stored  */

    /* Initialise the cache with the first four terms if empty. */
    if (!cache) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();              /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        computed = 4;
    }

    /* If the requested index is already known, return it. */
    if (n < computed)
        return cache[n];

    /* Ensure the array is large enough.  Grow exponentially. */
    if (n + 1 > cap) {
        size_t newcap = cap;
        while (newcap <= n)
            newcap <<= 1;                /* double until big enough   */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure       */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute the sequence from the first missing index up to n. */
    for (idx_t i = computed; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) - a(i-3) + 2*a(i-4)   */
        seq_t val = ((seq_t)2) * cache[i-1];
        val -= cache[i-3];
        val += ((seq_t)2) * cache[i-4];
        cache[i] = val;
    }
    computed = n + 1;
    return cache[n];
}
