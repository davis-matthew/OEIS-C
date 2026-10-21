/*
 * A287810  Number of septenary sequences of length n such that no two
 *          consecutive terms have distance 3.
 * https://oeis.org/A287810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=7, a(2)=41, a(3)=241, a(n) = 6*a(n-1) + a(n-2) -
 *            10*a(n-3) for n>3 (the recurrence from OEIS A287810)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* allocated size of cache   */
    static size_t computed = 0;  /* first n that has been computed (0 means none) */

    /* grow cache if necessary */
    if ((idx_t)n >= cap) {
        size_t newcap = cap ? cap * 2 : 4;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(*cache));
        if (!tmp) exit(1);           /* out of memory – abort */
        cache = tmp;
        cap   = newcap;
    }

    /* initialise base cases when needed */
    if (computed == 0) {            /* a(0) */
        cache[0] = 1;
        computed = 1;
    }
    if (computed == 1 && n >= 1) {  /* a(1) */
        cache[1] = 7;
        computed = 2;
    }
    if (computed == 2 && n >= 2) {  /* a(2) */
        cache[2] = 41;
        computed = 3;
    }
    if (computed == 3 && n >= 3) {  /* a(3) */
        cache[3] = 241;
        computed = 4;
    }

    /* compute further values using the recurrence */
    for (size_t k = computed; k <= (size_t)n; ++k) {
        cache[k] = 6 * cache[k-1] + cache[k-2] - 10 * cache[k-3];
    }
    computed = (size_t)n + 1;

    return cache[(size_t)n];
}
