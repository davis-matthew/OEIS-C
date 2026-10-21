/*
 * A250111  Number of orbits of size 2 in vertices of Fibonacci cube Gamma_n
 *          under the action of its automorphism group.
 * https://oeis.org/A250111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+2*a(n-2)-a(n-3)-a(n-5)-a(n-6) (n>7) Initial
 *            values: a(1)=1,a(2)=1,a(3)=1,a(4)=3,a(5)=4,a(6)=9,a(7)=13 The
 *            function memoises already computed terms in a static array
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc in C */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* n<1 is not used for the sequence */

    static seq_t *cache   = NULL;  /* cache[1..maxn] */
    static idx_t maxn     = 0;     /* largest index stored */

    /* --- initialise cache with the first 7 values --- */
    if (cache == NULL) {
        maxn = 7;
#if defined(__cplusplus)
        /* In C++ use new to construct the objects */
        cache = new seq_t[maxn + 1];
#else
        /* In C use malloc/realloc – cast is required for C++ */
        cache = (seq_t *)malloc((maxn + 1) * sizeof(seq_t));
#endif
        if (cache == NULL) return 0;  /* allocation failed */

        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 3;
        cache[5] = 4;
        cache[6] = 9;
        cache[7] = 13;
    }

    /* --- return if already computed --- */
    if (n <= maxn) {
        return cache[n];
    }

    /* --- grow cache to at least n --- */
    idx_t newmax  = n;
    seq_t *newcache;
#if defined(__cplusplus)
    newcache = new seq_t[newmax + 1];
    for (idx_t i = 1; i <= maxn; ++i) {
        newcache[i] = cache[i];
    }
    delete[] cache;
#else
    newcache = (seq_t *)realloc(cache, (newmax + 1) * sizeof(seq_t));
    if (newcache == NULL) return 0;  /* allocation failed */
    cache = newcache;
#endif

    /* --- compute missing terms using the recurrence --- */
    for (idx_t i = maxn + 1; i <= newmax; ++i) {
        newcache[i] = newcache[i - 1]
                      + 2 * newcache[i - 2]
                      - newcache[i - 3]
                      - newcache[i - 5]
                      - newcache[i - 6];
    }

    cache = newcache;
    maxn  = newmax;

    return cache[n];
}
