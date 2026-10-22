/*
 * A387623  a(n) = Sum_{k=0..floor(n/4)} 2^k * binomial(2*n-6*k,2*k).
 * https://oeis.org/A387623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} 2^k * binomial(2*n-6*k,2*k)
 *            Recurrence: a(n) = 2*a(n-1) - a(n-2) + 4*a(n-4) + 4*a(n-5) -
 *            4*a(n-8) Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=3,
 *            a(5)=13, a(6)=31, a(7)=57
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 60, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0..len-1] holds computed terms, capacity = cap */
    static seq_t *cache = NULL;
    static size_t len  = 0;   /* number of stored terms */
    static size_t cap  = 0;   /* allocated size */

    static const seq_t init[] = {1,1,1,1,3,13,31,57};
    const size_t init_len = sizeof(init)/sizeof(init[0]);

    /* Return precomputed initial values */
    if (n < init_len)
        return init[n];

    /* first allocation */
    if (cache == NULL) {
        cap = init_len;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return init[0];   /* unrecoverable, return any value */
        for (size_t i = 0; i < init_len; ++i)
            cache[i] = init[i];
        len = init_len;
    }

    /* ensure capacity for index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return init[0];   /* unrecoverable */
        cache = tmp;
        cap = new_cap;
    }

    /* compute new terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1] * (seq_t)2;
        val += cache[i-4] * (seq_t)4;
        val += cache[i-5] * (seq_t)4;
        val -= cache[i-2];
        val -= cache[i-8] * (seq_t)4;
        cache[i] = val;
    }
    len = (size_t)n + 1;

    return cache[n];
}
