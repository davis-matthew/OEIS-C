/*
 * A299592  Number of n X 5 0..1 arrays with every element unequal to 0, 1 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A299592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 56, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (A299592):
   a(1)=8, a(2)=8, a(3)=19, a(4)=53, a(5)=113, a(6)=256, a(7)=541,
   a(8)=1148, a(9)=2488;
   a(n)=a(n-1)+a(n-2)+3*a(n-3)+a(n-4)-a(n-5)-a(n-6)   for n>9
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset starts at 1 */
    if (n == 0)
        return 0;

    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;      /* allocated elements (including index 0) */
    static size_t sz      = 0;      /* highest calculated index */
    static int   init     = 0;      /* first initialisation flag */

    /* initialise cache and first 9 terms on first call */
    if (!init) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return 0;          /* allocation failure */
        for (size_t i = 0; i < cap; ++i)
            cache[i] = 0;

        /* preset known values (indices 1..9) */
        cache[1] = 8;   cache[2] = 8;   cache[3] = 19;   cache[4] = 53;
        cache[5] = 113; cache[6] = 256; cache[7] = 541;  cache[8] = 1148;
        cache[9] = 2488;
        sz = 9;
        init = 1;
    }

    /* grow the cached array if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 64)
            new_cap = 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;          /* allocation failure */
        /* initialise newly allocated part to zero */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* if the term is already computed, return it */
    if ((size_t)n <= sz)
        return cache[n];

    /* compute terms from (sz+1) up to n using the recurrence */
    for (size_t i = sz + 1; i <= (size_t)n; ++i) {
        /* a(i) = a(i-1)+a(i-2)+3*a(i-3)+a(i-4)-a(i-5)-a(i-6) */
        seq_t sum = cache[i - 1] + cache[i - 2];
        sum += cache[i - 3] * (seq_t)3;
        sum += cache[i - 4];
        sum -= cache[i - 5];
        sum -= cache[i - 6];
        cache[i] = sum;
    }

    sz = (size_t)n;
    return cache[n];
}
