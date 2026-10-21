/*
 * A276229  a(n+3) = -a(n+2) - 2*a(n+1) + a(n) with a(0)=0, a(1)=0, a(2)=1.
 * https://oeis.org/A276229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = -a(n+2) - 2*a(n+1) + a(n); Initial values: a(0)=0, a(1)=0, a(2)=1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 0;
    if (n == 2)
        return 1;

    /* Static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size of cache   */
    static size_t sz  = 0;   /* number of usable entries  */

    /* Initialise cache on first use */
    if (!cache) {
        cap = 64;                          /* initial allocation    */
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (!cache) abort();               /* fatal if malloc fails */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        sz = 3;
    }

    /* Grow the allocation if required */
    if ((size_t)n + 1 > cap) {
        size_t newcap = ((size_t)n + 1) * 2;
        if (newcap < 4) newcap = 4;        /* just in case */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = sz; i <= n; ++i) {
        /* a(i) = -a(i-1) - 2*a(i-2) + a(i-3) */
        cache[(size_t)i] = -cache[(size_t)(i - 1)]
                         - (seq_t)2 * cache[(size_t)(i - 2)]
                         + cache[(size_t)(i - 3)];
    }

    sz = (size_t)n + 1;
    return cache[(size_t)n];
}
