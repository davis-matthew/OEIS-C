/*
 * A397108  Lower (2,1) midsequence of (floor[n/2]^2) and (ceiling[n/2]^2);
 *          see Comments.
 * https://oeis.org/A397108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5), with
 *            initial values a(0)=0, a(1)=1, a(2)=3, a(3)=6, a(4)=12
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t  last  = (idx_t)-1; /* highest index computed so far */
    static size_t cap   = 0;           /* capacity of the array */

    /* Helper to allocate the cache in C or C++ */
    if (!cache) {
    #ifdef __cplusplus
        /* In C++, allocate a raw array so all objects are constructed */
        cache = new seq_t[5];
    #else
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
    #endif
        if (!cache) return 0;          /* out of memory – unlikely */
        cap   = 5;
        /* initialise the first five terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)12;
        last   = 4;
    }

    /* If the requested value is already cached, return it */
    if (n <= last) return cache[n];

    /* Ensure enough space for n+1 elements */
    if ((size_t)(n + 1) > cap) {
    #ifdef __cplusplus
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = new seq_t[newcap];
        /* copy existing values */
        for (idx_t i = 0; i <= last; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
        cap   = newcap;
    #else
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0; /* out of memory – unlikely */
        cache = tmp;
        cap   = newcap;
    #endif
    }

    /* Compute terms iteratively up to index n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = cache[i-1];
        term += cache[i-2] << 1;      /* 2 * a[i-2] */
        term += cache[i-5];
        term -= (cache[i-3] << 1);    /* 2 * a[i-3] */
        term -= cache[i-4];
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
